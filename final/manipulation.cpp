/**********************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"


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

ImageData* Manipulation::change(ImageData* inputImage)
{
     if(inputImage== NULL)
    {
        std::cout<< "Manipulation::change() parameter inputImageData shouldn't be null!" << std::endl;
        exit(-1);
    }
   
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;



    return imageData;
}
