/*************************************************************************
	> File Name: read.cpp
	> Author: Maoting Ren 
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 01:05:55 AM EDT
 ************************************************************************/

#include "read.h"
#include <string>
#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <iostream>

Read:: ImageData* Read::readImage(char* filename)
{
    ImageInput *in = ImageInput::open(filename);
    if(!in){
        std::cerr<<"Could not create: " << geterror();
        exit(-1);
    }
    ImageData* imageData = new ImageData();
    const ImageSpec &spec = in->spec();
    imageData->width = spec.width;
    imageData->height = spec.height;
    imageData->channels = spec.nchannels;
        
    std::cout << "x resolution: " <<  imageData->width << std::endl;
    std::cout << "y resolution: " << imageData->height << std::endl;
    std::cout << "num channels: " << imageData->channels << std::endl;
    imageData->pixels = new GLubyte[imageData->width*imageData->height*imageData->channels]; 
    if (!in->read_image (TypeDesc::UINT8, imageData->pixels)) {
       std::cerr << "Could not read pixels from " << filename;
       std::cerr << ", error = " << in->geterror() << std::endl;
       delete in;
       exit(-1);
    }

    if (!in->close ()) {
       std::cerr << "Error closing " << filename;
       std::cerr << ", error = " << in->geterror() << std::endl;
       delete in;
       exit(-1);
    }
    std::cout << "read succed!" << std::endl;
    delete in;
    return imageData;
}




