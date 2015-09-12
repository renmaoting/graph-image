/*************************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"
#include <string.h>
#include <algorithm>

//herizontally flip the image
Manipulation::ImageData& Manipulation::herizontalFlip(ImageData* inputImageData)
{
    copyImageData(inputImageData); 
    arrayToVector(inputImageData);
    for(int i = 0; i< inputImageData->height; i++)
    {
        reverse(dataVec1[i].begin(), dataVec1[i].end());
    }
    vectorToArray();
    std::cout << "herizontal flip succed!" << std::endl;
    return imageData;
}


//vertically flip the image
Manipulation::ImageData& Manipulation::verticalFlip(ImageData* inputImageData)
{
    copyImageData(inputImageData); 
    arrayToVector(inputImageData);
    reverse(dataVec1.begin(), dataVec1.end());
    vectorToArray();
    std::cout << "vertical flip succed!" << std::endl;
    
    return imageData;
}


//copy the image data from input image to member variable
bool Manipulation::copyImageData(Manipulation::ImageData* inputImageData)
{
    if(inputImageData == NULL)
    {
        std::cout << "input data is NULL in manipulation.cpp:15" << std::endl;
        exit(-1);
    }
    imageData.width = inputImageData->width;
    imageData.height = inputImageData->height;
    imageData.channels = inputImageData->channels;
    imageData.pixels = new GLubyte[inputImageData->width * inputImageData->height * inputImageData->channels];
    
    return true;
}


//convert the data from array to verctor so that I can flip the data easily by system function reverse()
bool Manipulation::arrayToVector(Manipulation::ImageData* inputImageData)
{
    if(inputImageData == NULL)
    {
        std::cout << "input data is NULL in manipulation.cpp:15" << std::endl;
        exit(-1);
    }
    dataVec2.clear();
    dataVec3.clear();
    dataVec1.clear();
    for(int i = 0; i < inputImageData->height; i++)
    {
        dataVec2.clear();
        for(int j =0; j < inputImageData->width; j++)
        {
            dataVec3.clear();
            for(int k = 0; k < inputImageData->channels; k++)
            {
                dataVec3.push_back(inputImageData->pixels[i*imageData.width*imageData.channels+
                                                          j*imageData.channels + k]);
            }
            dataVec2.push_back(dataVec3);
        }
        dataVec1.push_back(dataVec2);
    }
    return true;
}


//after flip the data, should write back data from vector to array
bool Manipulation::vectorToArray()
{
    for(int i =0; i< dataVec1.size(); i++)
    {
        for(int j =0; j< dataVec1[i].size(); j++)
        {
            for(int k =0; k <dataVec1[i][j].size(); k++)
            {
                imageData.pixels[i*imageData.width*imageData.channels+j*imageData.channels + k] = dataVec1[i][j][k];
            }
        }
    }
    return true; 
}


//get a single channel image, and 
//channel = 0 represent R
//channel = 1 represent G
//channel = 2 represent B
Manipulation::ImageData& Manipulation::getSingleChannel(ImageData* inputImageData, int channel)
{
    if(inputImageData == NULL)
    {
        std::cout << "input data is NULL in manipulation.cpp:15" << std::endl;
        exit(-1);
    }
    if(channel < 0 || channel >= 3)
    {
        std::cerr << "make sure your channel parameter is right!" << std::endl;
        exit(-1);
    }
    
    if(inputImageData->channels < 3)
    {
        std::cerr << "can't extract single channel because this picture have not RGB channels!" <<std::endl;
        exit(-1);
    }
    copyImageData(inputImageData);
    arrayToVector(inputImageData);

    int cnt = 0;
    for(int i =0; i < dataVec1.size(); i++)
    {
        for(int j =0; j< dataVec1[i].size(); j++)
        {
            for(int k =0; k< dataVec1[i][j].size(); k++)
            {
                imageData.pixels[cnt++] = dataVec1[i][j][channel]; 
            }
        }
    }
    return imageData;
}






