/**********************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"
#include <cstring>


ImageData* Manipulation::verticalFlip(ImageData* inputImage)
{
    // this function will vertically flip the image
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::verticalFlip() parameter inputImageData shouldn't be null!" << std::endl;
        exit(-1);
    }
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;

    int cnt =0;
    for(int i =0; i< imageData->height; i++ )
        for(int j = 0; j< imageData->width * imageData->channels; j++)
            imageData->pixels[cnt++] = inputImage->pixels[(inputImage->height -i - 1)* inputImage->width* inputImage->channels +j];

    return imageData;
}

ImageData* Manipulation::horizFlip(ImageData* inputImage)
{
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::horizFlip() parameter inputImageData shouldn't be null!" << std::endl;
        exit(-1);
    }
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;



    return imageData;
}

ImageData* change(ImageData* inputImage, int radius, int intensityLevel, const std::vector<Point*>& pVec, int buttonType)
{
    // this function mainly convert a normal image to an oil image
    if(inputImage== NULL || radius <= 0)
    {
        std::cout<< "Manipulation::change() parameter inputImageData shouldn't be null!, or radius should >= 0" << std::endl;
        exit(-1);
    }
   
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;
    float *pixels = inputImage->pixels;

    for(int i =0; i < imageData->height; i++)
    {
        for(int j =0; j< imageData->width; j++)
        {
            int intensityHash[256],  maxNum=0, maxIndex =0;
            float rAvg[256], gAvg[256], bAvg[256];
            memset(intensityHash, 0, sizeof(intensityHash));
            memset(rAvg, 0, sizeof(rAvg));
            memset(gAvg, 0, sizeof(gAvg));
            memset(bAvg, 0, sizeof(bAvg));

            // from [i - radius, j - radius] to [i + radius, j + radius]
            for(int m = -radius; m <= radius ; m++)
            {
                for(int n = -radius; n <= radius; n++)
                {
                    // judge if this pixel is in the image
                    if((i + m >=0 && i + m < imageData->height) && (j + n >= 0 && j +n < imageData->width))
                    {
                        int currentIntensity;
                        float sum =0;
                        for(int k =0; k < inputImage->channels; k++)
                            sum+= pixels[(i+m)* inputImage->width * inputImage->channels + (j+n)*inputImage->channels + k];
                        currentIntensity = sum* intensityLevel /3;// compute intensity
                        intensityHash[currentIntensity]++;
                        if(intensityHash[currentIntensity] > maxNum)// find the most repeated intesity
                        {
                            maxNum = intensityHash[currentIntensity];
                            maxIndex = currentIntensity; 
                        }

                        // accumulate the r, g, b channels
                        rAvg[currentIntensity] += pixels[(i+m)* inputImage->width * inputImage->channels + (j+n)*inputImage->channels];
                        gAvg[currentIntensity] += pixels[(i+m)* inputImage->width * inputImage->channels + (j+n)*inputImage->channels +1];
                        bAvg[currentIntensity] += pixels[(i+m)* inputImage->width * inputImage->channels + (j+n)*inputImage->channels +2];
                    }
                }
            }

            // assign average value to the new iamge
            imageData->pixels[i* inputImage->width * inputImage->channels + j*inputImage->channels]    = rAvg[maxIndex] / intensityHash[maxIndex];
            imageData->pixels[i* inputImage->width * inputImage->channels + j*inputImage->channels +1] = gAvg[maxIndex] / intensityHash[maxIndex];
            imageData->pixels[i* inputImage->width * inputImage->channels + j*inputImage->channels +2] = bAvg[maxIndex] / intensityHash[maxIndex];
        }
    }    

    return imageData;
}
