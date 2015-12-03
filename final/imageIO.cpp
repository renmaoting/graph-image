/*************************************************************************
	> File Name: imageIO.cpp
	> Author: Maoting Ren
	> Mail: rmtustc@gmail.com
	> Created Time: Sun 04 Oct 2015 12:11:58 PM EDT
 ************************************************************************/

#include "imageIO.h"
#include <string>
#include <fstream>
#include <OpenImageIO/imageio.h>

OIIO_NAMESPACE_USING


ImageData* ImageIO::readImage(char* filename)//read the image information into a structure
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
        
    std::cout << "width : " <<  imageData->width << std::endl;
    std::cout << "height : " << imageData->height << std::endl;
    std::cout << "num channels: " << imageData->channels << std::endl;
    imageData->pixels = new float[imageData->width*imageData->height*imageData->channels]; 
    if (!in->read_image (TypeDesc::FLOAT, imageData->pixels)) {
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

bool ImageIO::writeData(char* filename, ImageData* originalImageData)
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
    ImageSpec spec (originalImageData->width, originalImageData->height, originalImageData->channels, TypeDesc::FLOAT);
    out->open(filename, spec);

    //it is possible that this TypeDesc does not match the ImageSpec, 
    //in which case it will convert the raw data into the spec. 
    //But it MUST match the datatype of raw data
    out->write_image(TypeDesc::FLOAT, originalImageData->pixels);

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

