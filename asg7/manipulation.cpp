/**********************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"
#include <string.h>
#include <algorithm>
#include <cmath>
#include <stdlib.h>
#include <limits.h>
#include <fstream>
#include "Vector.h"


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

void Manipulation::rotate(Matrix3x3 &M, float theta)
{
    // Multiply M by a rotation matrix of angle theta
    int row, col;
    Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    double rad, c, s;

    rad = PI * theta / 180.0;
    c = cos(rad);
    s = sin(rad);

    R[0][0] = c;
    R[0][1] = -s;
    R[1][0] = s;
    R[1][1] = c;

    Matrix3x3 Prod = R * M;

    for(row = 0; row < 3; row++) {
        for(col = 0; col < 3; col++) {
            M[row][col] = Prod[row][col];
        }
    }
}

void Manipulation::translate(Matrix3x3 &M, int dx, int dy)
{
    // Multiply M by a translate matrix
    Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    R[0][2] = dx;
    R[1][2] = dy;
    Matrix3x3 prod = R * M;

    for(int row =0; row < 3; row++)
    {
        for(int col =0; col < 3; col++)
        {
            M[row][col] = prod[row][col];
        }
    }
}

void Manipulation::shear(Matrix3x3 &M, float shx, float shy) 
{
    // Multiply M by a shear matrix
    Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    R[0][1] = shx;
    R[1][0] = shy;
    Matrix3x3 prod = R * M;
 
    for(int row =0; row < 3; row++)
    {
        for(int col =0; col < 3; col++)
        {
            M[row][col] = prod[row][col];
        }
    }   
}
    
void Manipulation::scale(Matrix3x3 &M, float sx, float sy)
{
    // Multiply M by a scale matrix
    Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    R[0][0] = sx;
    R[1][1] = sy;
    Matrix3x3 prod = R * M;
 
    for(int row =0; row < 3; row++)
    {
        for(int col =0; col < 3; col++)
        {
            M[row][col] = prod[row][col];
        }
    }   
}

void Manipulation::perspective(Matrix3x3 &M, float px, float py)
{
    // Multiply M by a perspective matrix
    Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    R[2][0] = px;
    R[2][1] = py;
    Matrix3x3 prod = R * M;
 
    for(int row =0; row < 3; row++)
    {
        for(int col =0; col < 3; col++)
        {
            M[row][col] = prod[row][col];
        }
    } 
}

ImageData* Manipulation::fwdTransform(ImageData* inputImage, Matrix3x3& M, int& leftMost, int& topMost)
{
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::fwdTransform() parameter inputImageData shouldn't be null !" << std::endl;
        exit(-1);
    }

    ImageData* imageData = new ImageData();
    leftMost = INT_MAX;
    int rightMost = INT_MIN;
    topMost = INT_MAX;
    int bottomMost = INT_MIN; 
    for(int i =0; i<= inputImage->height; i+= (inputImage->height))
    {
        for(int j =0; j<= inputImage->width; j+= (inputImage->width))
        {
            // forward transform
            Vector3d vecSrc(j, i, 1);// source position
            Vector3d vecDst = M * vecSrc;// destination position
            
            if(vecDst[2] != 1 && vecDst[2] != 0)
            {
                vecDst[0] /= vecDst[2];
                vecDst[1] /= vecDst[2];
                vecDst[2] = 1;
            }

            if(vecDst[1] > bottomMost)// determine the boundary of new image
                bottomMost = vecDst[1];
            if(vecDst[1] < topMost)
                topMost = vecDst[1];
            if(leftMost > vecDst[0])
                leftMost = vecDst[0];
            if(rightMost < vecDst[0])
                rightMost = vecDst[0];
        }
    }

    imageData->width = rightMost - leftMost ;// calculate the width
    imageData->height = bottomMost - topMost ;// calculate the height
    imageData->channels = inputImage->channels;// and channel
    std::cout << "width, height, channels of new image is = " << imageData->width << " " << imageData->height << " "<<imageData->channels << std::endl;
    imageData->pixels = new float[imageData->width * imageData->height * imageData->channels];
 
    return imageData;
}

float Manipulation::bilinear(ImageData* inputImage, Vector3d& vecSrc, int k)
{
    int x = floor(vecSrc[0]);
    int y = floor(vecSrc[1]);
    int a = vecSrc[0] - x;
    int b = vecSrc[1] - y;

    float value4 = inputImage->pixels[(y+1) * inputImage->width * inputImage->channels + (x+1)* inputImage->channels + k];
    float value3 = inputImage->pixels[(y+1) * inputImage->width * inputImage->channels + x* inputImage->channels + k];
    float value2 = inputImage->pixels[y * inputImage->width * inputImage->channels + (x+1)* inputImage->channels + k];
    float value1 = inputImage->pixels[y * inputImage->width * inputImage->channels + x* inputImage->channels + k];

    Vector2d vecF(1- b,b);
    Vector2d vecB(1- a,a);
    Matrix2x2 m(value1, value2, value3, value4);
    float value = vecF * m * vecB;
    return value;
}

float Manipulation::linear(ImageData* inputImage, Vector3d& vecSrc, int k)
{

}

ImageData* Manipulation::warper(ImageData* inputImage, Matrix3x3 &M, bool flag)
{
    // create a new image and then fill it by input image as the warper matrix specified    
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::warper() parameter inputImageData shouldn't be null!" << std::endl;
        exit(-1);
    }

    int leftMost;
    int topMost;
    ImageData* imageData = fwdTransform(inputImage, M, leftMost, topMost);
    
    Matrix3x3 bwMap = M.inv();// M^(-1)
    Vector3d vecSrc;// source pixels position    
    for(int i =0; i< imageData->height; i++)
    {
        for(int j =0; j< imageData->width; j++)
        {
            Vector3d vecDes(j+leftMost,i+topMost, 1); // destination position
            vecSrc = bwMap * vecDes;
            
            if(vecSrc[2] != 0 && vecSrc[2] != 1)// normalization when the w component is not 1
            {
                vecSrc[0] /= vecSrc[2];
                vecSrc[1] /= vecSrc[2];
                vecSrc[2] /= 1;
            }

            // if the position beyond the original image, then fill in 0
            //int x = round(vecSrc[0]) ;// round to int type
            //int y = round(vecSrc[1]);

            if(vecSrc[1] >= inputImage->height || vecSrc[1] < 0 || vecSrc[0] < 0 || vecSrc[0] >= inputImage->width)// handle the pixels that out of original image
                for(int k =0; k< imageData->channels; k++ )
                    imageData->pixels[i*imageData->width*imageData->channels + j* imageData->channels +k] = 0;
            else
                for(int k =0; k < imageData->channels; k++)// inverse mapping
                {
                    // use bilinear interpolation
                    if(flag == 0)
                    {
                    
                    }
                    else if(flag == 1)
                    {
                        float value = bilinear(inputImage, vecSrc, k);
                        imageData->pixels[i *imageData->width*imageData->channels + j* imageData->channels +k] = value;
                    }
                }
        }
    }    

    return imageData;
}

ImageData* Manipulation::twirl(ImageData* inputImage, float s, float cx, float cy)
{
    //this function will twirl image
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::twirl() parameter inputImageData shouldn't be null!" << std::endl;
        exit(-1);
    }
    if( cx <= 0 || cx >= 1 || cy <= 0 || cy >= 1)
    {
        std::cout<< "Manipulation::twirl() parameter cx, cy should be between (0, 1)!" << std::endl;
        exit(-1);
    }
    
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;// this is a bad idea, which if use unproplely, may cause bugs that can hardly to find out
    std::cout << "new image width = " << imageData->width << " height = " << imageData->height<< " channels =" << imageData->channels << std::endl;

    int cx2 = cx * inputImage->width;//  centerX
    int cy2 = cy * inputImage->height;// centerY
    int md = min(inputImage->width, inputImage->height);
    float theta;
    for(int i =0; i < imageData->height; i++)
    {
        for(int j =0; j < imageData->width; j++)
        {
            // use inverse mapping
            // md = min(WIDTH, HEIGHT)
            float r = sqrt(pow(j-cx2, 2) + pow(i -cy2, 2) );
            float a = s * (r - md)/md;
            theta = atan2(i - cy2, j - cx2) ;
            int u = round(r * cos(theta + a) + cx2);// get original pixel position
            int v = round(r * sin(theta + a) + cy2);

            if(u < 0 || u >= inputImage->width || v <0 || v >= inputImage->height)// handle the pixels that out of original image
            {
                for(int k =0; k < imageData->channels; k++)
                {
                    imageData->pixels[i* imageData->width * imageData->channels + j* imageData->channels + k] = 0; 
                }
            }
            else
            {
                for(int k =0; k < imageData->channels; k++)// inverse mapping
                {
                    imageData->pixels[i* imageData->width * imageData->channels + j* imageData->channels + k] = 
                        inputImage->pixels[v * inputImage->width * inputImage->channels + u * inputImage->channels + k]; 
                }
            }
        }
    }

    return imageData;
}
