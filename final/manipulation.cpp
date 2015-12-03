/**********************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"
#include <cstring>
#include <cmath>


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

bool Manipulation::checkQuadra(const std::vector<Point*>& pVec, int x, int y)
{
    if(x > pVec[0]->x && y > pVec[0]->y && x < pVec[1]->x && y < pVec[1]->y)
        return true;
    return false;
}

int Manipulation::checkTriangle(const std::vector<Point*>& pVec,int x, int y)
{
    Point* p = new Point(x, y);
    Point* a = pVec[0];
    Point* b = pVec[1];
    Point* c = pVec[2];
    bool res = toLeft(a, b, p);
    if(res != toLeft(b, c, p))
        return false;
    if(res != toLeft(c, a, p))
        return false;
    if(cross(a,b,c) == 0)
        return false;
    return true;
}

//bool Manipulation::checkTriangle(const std::vector<Point*>& pVec, int x, int y)
//{
 /*   //this function is used to judge if (x, y) is in this triangle area
    // A(a1, a2), B(b1, b2), C(c1, c2)
    // BC: fa(x, y) = 0;
    // AC: fb(x, y) = 0;
    // AB: fc(x, y) = 0;
    // f(x,y) = (y - y2)(x1 - x2) - (x - x2)(y1 - y2)
    //
    //if (x, y) is in this triangle area, then it should satisfy this condition
    //fa(x, y) * fa(a1, a2) > 0
    //fb(x, y) * fb(b1, b2) > 0
    //fc(x, y) * fc(c1, c2) > 0
    int m1 = (    y      - pVec[1]->y) *(pVec[0]->x - pVec[1]->x) - (     x     - pVec[1]->x)*(pVec[0]->y - pVec[1]->y);    
    int m2 = (pVec[2]->y - pVec[1]->y) *(pVec[0]->x - pVec[1]->x) - (pVec[2]->x - pVec[1]->x)*(pVec[0]->y - pVec[1]->y);
    int n1 = (    y      - pVec[2]->y) *(pVec[0]->x - pVec[2]->x) - (     x     - pVec[2]->x)*(pVec[0]->y - pVec[2]->y);    
    int n2 = (pVec[1]->y - pVec[2]->y) *(pVec[0]->x - pVec[2]->x) - (pVec[1]->x - pVec[2]->x)*(pVec[0]->y - pVec[2]->y);    
    int k1 = (    y      - pVec[2]->y) *(pVec[1]->x - pVec[2]->x) - (     x     - pVec[2]->x)*(pVec[1]->y - pVec[2]->y);    
    int k2 = (pVec[0]->y - pVec[2]->y) *(pVec[1]->x - pVec[2]->x) - (pVec[0]->x - pVec[2]->x)*(pVec[1]->y - pVec[2]->y);    
    if(m1*m2 > 0 && n1*n2 > 0 && k1*k2 > 0)
        return true;
    return false;*/
//}

bool Manipulation::checkCircle(const std::vector<Point*>& pVec, int x, int y)
{
    int d1 = pow(pVec[0]->x - pVec[1]->x, 2.0) + pow(pVec[0]->y - pVec[1]->y, 2.0);// this is r*r
    int d2 = pow(pVec[0]->x - x, 2.0) + pow(pVec[0]->y - y, 2.0);// this is the distance* distance from center
    if(d1 > d2)
        return true;
    return false;
}

ImageData* Manipulation::change(ImageData* inputImage, int radius, int intensityLevel, const std::vector<Point*>& pVec, int buttonType)
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
            // this pixel is not in the selected area
            if((buttonType == 0 && !checkQuadra(pVec, j, i))        // when the area is a quadra
                || (buttonType == 1 && !checkTriangle(pVec, j, i))  // when the area is a triangle
                || (buttonType == 2 && !checkCircle(pVec, j, i)))   // when the area is a circle
            {
                for(int k =0; k < imageData->channels; k++)
                    imageData->pixels[i* imageData->width * imageData->channels + j*imageData->channels +k] = 
                        inputImage->pixels[i* imageData->width * imageData->channels + j*imageData->channels +k];
                continue;
            }

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
