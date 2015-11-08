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
        std::cout<< "Manipulation::verticalFlip() parameter inputImageData shouldn't be null in Manipulation::verticalFlip!" << std::endl;
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

void Manipulation::shear(Matrix3x3 &M, int shx, int shy) 
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
    
void Manipulation::scale(Matrix3x3 &M, int sx, int sy)
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

ImageData* Manipulation::warper(ImageData* inputImage, Matrix3x3 &M)
{
    // create a new image and then fill it by input image as the warper matrix specified    
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::warper() parameter inputImageData shouldn't be null in Manipulation::warper!" << std::endl;
        exit(-1);
    }
    ImageData* imageData = fwdTransform(inputImage, M); // get a new image from forward transform
    
    Matrix3x3 bwMap = M.inv();// M^(-1)
    Vector3d vecSrc;// source pixels position    
    for(int i =0; i< imageData->height; i++)
    {
        for(int j =0; j< imageData->width; j++)
        {
            Vector3d vecDes(i, j, 1); // destination position
            vecSrc = bwMap * vecDes;

            // if the position beyond the original image, then fill in 0
            if(vecSrc[0] >= imageData->height || vecSrc[0] < 0 || vecSrc[1] < 0 || vecSrc[1] > imageData->width)
                for(int k =0; k< imageData->channels; k++ )
                    imageData->pixels[i*imageData->width*imageData->channels + j* imageData->channels +k] = 0;
            else
                for(int k =0; k < imageData->channels; k++)
                {
                    int x = round(vecSrc[0]);// round to int type
                    int y = round(vecSrc[1]);
                    imageData->pixels[i*imageData->width*imageData->channels + j* imageData->channels +k] 
                        = inputImage->pixels[x * inputImage->width * inputImage->channels + y* inputImage->channels + k];
                }
        }
    }    


    return imageData;
}

ImageData* Manipulation::fwdTransform(ImageData* inputImage, Matrix3x3 &M)
{
    // this function will use forward transform to create a new image
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::warper() parameter inputImageData shouldn't be null in Manipulation::warper!" << std::endl;
        exit(-1);
    }

    ImageData* imageData = new ImageData();
    int leftMost = INT_MAX;
    int rightMost = INT_MIN;
    int topMost = INT_MAX;
    int bottomMost = INT_MIN; 
    Vector3d vecDst(0,0,0);// destination position
    for(int i =0; i< inputImage->height; i+= inputImage->height)
    {
        for(int j =0; j< inputImage->width; j+= inputImage->width)
        {
            Vector3d vecSrc(i, j, 1);// source position
            vecDst = M * vecSrc;
            if(vecDst[0] > bottomMost)// determine the boundary of new image
                bottomMost = vecDst[0];
            if(vecDst[0] < topMost)
                topMost = vecDst[0];
            if(leftMost > vecDst[1])
                leftMost = vecDst[1];
            if(rightMost < vecDst[1])
                rightMost = vecDst[1];
        }
    }

    imageData->width = rightMost - leftMost;// calculate the width
    imageData->height = bottomMost - topMost;// calculate the height
    imageData->channels = inputImage->channels;// and channel
    imageData->pixels = new float[imageData->width * imageData->height * imageData->channels];

    return imageData;
}

ImageData* Manipulation::twirl(ImageData* inputImage, float s, float cx, float cy)
{
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::warper() parameter inputImageData shouldn't be null in Manipulation::warper!" << std::endl;
        exit(-1);
    }
    
    ImageData* imageData = new ImageData();
    imageData = inputImage;


    return imageData;
}
