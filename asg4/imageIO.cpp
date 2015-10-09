/*************************************************************************
	> File Name: imageIO.cpp
	> Author: Maoting Ren
	> Mail: rmtustc@gmail.com
	> Created Time: Sun 04 Oct 2015 12:11:58 PM EDT
 ************************************************************************/

#include "imageIO.h"
#include <string>
#include <OpenImageIO/imageio.h>
#include <fstream>

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

bool ImageIO::readFilt(std::vector<std::vector<double> >& vecFilt,char* filename)
{
    if(filename == NULL)
    {
        std::cout << "ImageIO::readFilt filename shouldn't be NULL" << std::endl;
        exit(-1);
    }
    int N;
    std::ifstream fin;
    fin.open(filename);
    vecFilt.clear();
    fin >> N;
    for(int i =0; i < N; i++)
    {
        std::vector<double> vec;
        double value;
        for(int j =0; j < N; j++)
        {
            fin>> value;
            vec.push_back(value); 
        }
        vecFilt.push_back(vec);
        vec.clear();
    }
    return true;
}