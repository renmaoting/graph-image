/*************************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"
#include <string.h>
#include <algorithm>
#include "math.h"
#include <stdlib.h>


//vertically flip the image
ImageData* Manipulation::verticalFlip(ImageData* inputImageData)
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

ImageData* Manipulation::filt(ImageData* inputImage, const std::vector<std::vector<double> >& vecFilt)
{
    if(inputImage == NULL || vecFilt.size() == 0)
    {
        std::cout << "Manipulation::filt inputImage shouldn't be NULL, or vector should not be NULL!" << std::endl;
        exit(0);
    }    
    ImageData* imageData;
    imageData = new ImageData();
    int width = inputImage->width;
    int height = inputImage->height;
    int channels = inputImage->channels;
    GLubyte* pixels = inputImage->pixels;
    imageData->pixels = new GLubyte[height * width * channels];
    imageData->height = height;
    imageData->width = width;
    imageData->channels = channels;
    
    for(int i =0; i < height; i++)
    {
        for(int j =0; j < width; j++)
        {
            for(int k =0; k < channels; k++)
            {
                double cnt =0, totalPositivetWeight = 0, totalNegativeWeight = 0;
                int N = vecFilt.size();
                for(int x = 0; x < N; x++)
                {
                    for(int y = 0; y < N; y++)
                    {
                        if(vecFilt[N-x-1][N-y-1] > 0)//get positive magnitude
                            totalPositivetWeight += vecFilt[N-x-1][N-y-1];
                        if(vecFilt[N-x-1][N-y-1] < 0)//get negative magnitude
                            totalNegativeWeight += vecFilt[N-x-1][N-y-1];
                        int m = i + x - N/2;
                        int n = j + y - N/2;
                        if(m < 0 || n < 0 || m >= height || n >= width)// in the boundary, I just set the outside pixel as 0
                            continue;
                        // in this place, I use reflection
                        cnt += vecFilt[N-x-1][N-y-1] * pixels[(i + x - N/2)* width * channels + (j + y - N/2)*channels + k];
                    }
                }

                // get the max of positive magnitude and negative magnitude as factor
                double factor = totalPositivetWeight>(-totalNegativeWeight)?totalPositivetWeight:(-totalNegativeWeight);
                if(factor == 0)
                    factor = 1;// this code will guarantee factor not be 0
                int value = cnt/factor;

                if(value <= 0) value = 0;// normalization
                if(value > 255) value = 255;
                imageData->pixels[i*width*channels + j*channels +k] = value;
            }
        }
    }

    return imageData;
}

bool Manipulation::gabor(std::vector<std::vector<double> >& vecFilt, int theta, double sigma, double period)
{
    //generate the gabor filter
    vecFilt.clear();
    for(int i =0; i< 2*sigma + 1; i++)
    {
        double value;
        std::vector<double> vec;
        for(int j =0; j< 2*sigma + 1; j++)
        {
            double x = (j - sigma)*cos(theta*3.14/180) + (i - sigma)* sin(theta*3.14/180);
            double y =-(j - sigma)*sin(theta*3.14/180) + (i - sigma)* cos(theta*3.14/180);
            value = exp(-(x*x + y*y) / (2 * sigma*sigma)) * cos(2*3.14 * x / period); 
            vec.push_back(value);
        }
        vecFilt.push_back(vec);
        vec.clear();
    }

    return true;
}






