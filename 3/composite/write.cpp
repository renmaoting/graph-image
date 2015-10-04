/*************************************************************************
	> File Name: write.h
	> Author: Maoting Ren 
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 01:57:16 AM EDT
 ************************************************************************/

#include "write.h"
#include <string>
#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <iostream>

bool Write::writeData(char* filename, ImageData* originalImageData)
{
    if(filename == NULL)
    {
        std::cerr << "invalid filename to write" << std::endl;
        exit(-1);
    }
    ImageOutput *out = ImageOutput::create(filename);
    if (!out) {
       std::cerr << "Could not create: " << geterror();
       exit(-1);
    }

    if(originalImageData == NULL)
    {
        std::cerr << "there is nothing to write!"<< std::endl;
        exit(-1);
    }

    //create the ImageSpec that describes how you will write the output data
    ImageSpec spec (originalImageData->width, originalImageData->height, originalImageData->channels, TypeDesc::UINT8);
    out->open(filename, spec);

    //it is possible that this TypeDesc does not match the ImageSpec, 
    //in which case it will convert the raw data into the spec. 
    //But it MUST match the datatype of raw data
    out->write_image(TypeDesc::UINT8, originalImageData->pixels);

    if (!out->close ()) {
       std::cerr << "Error closing " << filename;
       std::cerr << ", error = " << out->geterror() << std::endl;
       delete out;
       exit(-1);
    }
    delete out; 
    std::cout<<"write succed!"<<std::endl;
    

    return true;
}


