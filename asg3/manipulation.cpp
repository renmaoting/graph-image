/*************************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"
#include <string.h>
#include <algorithm>
#include "colorwindow.h"
#include "math.h"

#define min(x1,x2)  ((x1)<(x2)?(x1):(x2))
#define max(x1,x2)  ((x1)>(x2)?(x1):(x2))

//vertically flip the image
Manipulation::ImageData* Manipulation::verticalFlip(ImageData* inputImageData)
{
    if(inputImageData == NULL)
    {
        std::cout<< "Manipulation::verticalFlip() parameter inputImageData shouldn't be null in Manipulation::verticalFlip!" << std::endl;
        exit(-1);
    }
    int width = inputImageData->width;
    int height = inputImageData->height;
    int channels = inputImageData->channels;
    ImageData* imageData = new ImageData();
    imageData->width = width;
    imageData->height = height;
    imageData->channels = channels;
    GLubyte* pixels = inputImageData->pixels;
    imageData->pixels = new GLubyte[width* height * channels];
    int cnt =0;
    for(int i =0; i< height; i++ )
    {
        for(int j = 0; j< width*channels; j++)
        {
            imageData->pixels[cnt++] = pixels[(height -i - 1)* width* channels +j];
        }
    }
    
    return imageData;
}

Manipulation::ImageData* Manipulation::masking(ImageData* inputImageData)
{
    if(inputImageData == NULL)
    {
        std::cout << "Manipulation::msking() parameter inputImageData should not be null" << std::endl;
        exit(-1);
    } 
    ImageData* imageData = new ImageData();
    int width = inputImageData->width;
    int height = inputImageData->height;
    int channels = inputImageData->channels;
    imageData->width = width;
    imageData->height = height;
    imageData->channels = 4;
    GLubyte* pixels = inputImageData->pixels;
    imageData->pixels = new GLubyte[width* height * 4];
    float h, s, v;
    for(int i = 0; i< height; i++)
    {
        for(int j = 0; j< width; j++)
        {
            imageData->pixels[i*width* 4 +j* 4] = pixels[i*width*channels + j*channels]; 
            imageData->pixels[i*width* 4 +j* 4 + 1] = pixels[i*width*channels + j*channels + 1]; 
            imageData->pixels[i*width* 4 +j* 4 + 2] = pixels[i*width*channels + j*channels + 2]; 
            RGBtoHSV(pixels[i*width*channels + j*channels], pixels[i*width*channels +j*channels +1], pixels[i*width*channels +j*channels +2], h, s, v);
            if(h > 70 && h < 160)
            {
                imageData->pixels[i*width* 4 +j* 4 + 3] = 0; 
            }
            else
            {
                imageData->pixels[i*width* 4 +j* 4 + 3] = 255; 
            }
        }
    } 
    return imageData; 
}

void Manipulation::RGBtoHSV ( unsigned char r, unsigned char g, unsigned char b, float &h, float &s, float &v )
{
    float red, green, blue;
    float maxc, minc, delta;

    // r, g, b to 0 - 1 scale
    red = r / 255.0; green = g / 255.0; blue = b / 255.0;  

    maxc = max(max(red, green), blue);
    minc = min(min(red, green), blue);

    v = maxc;        // value is maximum of r, g, b

    if(maxc == 0){    // saturation and hue 0 if value is 0
        s = 0;
        h = 0;
    } else {
    s = (maxc - minc) / maxc; 	// saturation is color purity on scale 0 - 1

    delta = maxc - minc;
    if(delta == 0)           // hue doesn't matter if saturation is 0 
        h = 0;
    else
    {
        if(red == maxc)       // otherwise, determine hue on scale 0 - 360
            h = (green - blue) / delta;
        else if(green == maxc)
            h = 2.0 + (blue - red) / delta;
        else // (blue == maxc)
            h = 4.0 + (red - green) / delta;
        h = h * 60.0;
        if(h < 0)
            h = h + 360.0;
        }
    }
}

void Manipulation::HSVtoRGB ( float h, float s, float v, unsigned char &r, unsigned char &g, unsigned char &b )
{
    int i;
	float f, p, q, t, red, green, blue;

    if (s == 0) {
        // grey
        red = green = blue = v;
    }
    else
    {
        h /= 60.0;
        i = (int) floor(h);
        f = h - i;
        p = v * (1-s);
        q = v * (1-s*f);
        t = v * (1 - s * (1 - f));

        switch (i) {
            case 0:
                red = v;
                green = t;
                blue = p;
                break;
            case 1:
                red = q;
                green = v;
                blue = p;
                break;
            case 2:
                red = p;
                green = v;
                blue = t;
                break;
            case 3:
                red = p;
                green = q;
                blue = v;
                break;
            case 4:
                red = t;
                green = p;
                blue = v;
                break;
            default:
                red = v;
                green = p;
                blue = q;
                break;
        }
    }

    r = (unsigned char) (red*255.0 + .5);
    g = (unsigned char) (green*255.0 + .5);
    b = (unsigned char) (blue*255.0 + .5);
}

Manipulation::ImageData*  Manipulation::pvMsking(ImageData* inputImageData, float& k)
{
    if(inputImageData == NULL)
    {
        std::cout << "Manipulation::pvMsking() parameter inputImageData should not be null" << std::endl;
        exit(-1);
    }
    
    ImageData* imageData = new ImageData();
    int width = inputImageData->width;
    int height = inputImageData->height;
    int channels = inputImageData->channels;
    imageData->width = width;
    imageData->height = height;
    imageData->channels = 4;
    GLubyte* pixels = inputImageData->pixels;
    imageData->pixels = new GLubyte[width* height * 4];
     
    for(int i =0; i< height; i++)
    {
        for(int j =0; j< width; j++)
        {
            int index = i*width*channels + j*channels;
            imageData->pixels[index] = pixels[index]; 
            imageData->pixels[index + 1] = pixels[index + 1]; 
            imageData->pixels[index + 2] = pixels[index + 2]; 
            if(pixels[index + 1] > pixels[index + 2] * k )
            {
                imageData->pixels[index + 3] = 0;
            }
            else
            {
                imageData->pixels[index +3] = 255;
            }
        }
    }    
    return imageData;
}
    
void Manipulation::ssMsking(ImageData* inputImageData)
{
    if(inputImageData == NULL)
    {
        std::cout << "Manipulation::ssMsking() parameter inputImageData should not be null" << std::endl;
        exit(-1);
    }
    int width = inputImageData->width;
    int height = inputImageData->height;
    int channels = inputImageData->channels;
    GLubyte* pixels = inputImageData->pixels;
    for(int i =0; i< height; i++)
    {
        for(int j =0; j< width; j++)
        {
            int r = pixels[i*width*channels + j*channels]; 
            int g = pixels[i*width*channels + j*channels +1 ]; 
            int b = pixels[i*width*channels + j*channels + 2];
            pixels[i*width*channels + j*channels + 2] = min(r, min(g, b));    
        }
    }
}

Manipulation::ImageData* Manipulation::compositeByMasking(ImageData* AImage, ImageData* BImage)
{
    if(AImage == NULL || BImage == NULL)
    {
        std::cout << "Manipulation::compositeByMasking() parameter inputImageData should not be null" << std::endl;
        exit(-1);
    }

    ImageData* aImage = NULL;
    aImage = masking(AImage);
    
    ImageData* imageData = new ImageData();
    int width = BImage->width;
    int height = BImage->height;
    int channels = BImage->channels;
    imageData->width = width;
    imageData->height = height;
    imageData->channels = channels;
    GLubyte* pixels = BImage->pixels;
    imageData->pixels = new GLubyte[width* height * channels];
    for(int i = 0; i < height; i++)
    {
        for(int j =0; j < width; j++)
        {
            int index = i*width*channels + j*channels; 
            for(int k =0; j< channels -1; j++)
            {
                imageData->pixels[index + k] = aImage->pixels[index + k]* aImage->pixels[index + 3] + BImage->pixels[index + k] *(1 - aImage->pixels[index + 3]);
            }
            imageData->pixels[index + 3] = 255;
        }
    }

    return imageData;
}






