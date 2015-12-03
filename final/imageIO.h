/*************************************************************************
	> File Name: imageIO.h
	> Author: Maoting Ren
	> Mail: rmtustc@gmail.com
	> Created Time: Sun 04 Oct 2015 12:11:58 PM EDT
 ************************************************************************/
#ifndef IMAGEIO_H
#define IMAGEIO_H

#include <GL/glut.h>

typedef struct Data{
    int width;
    int height;
    int channels;
    float* pixels;
    Data& operator =(const Data& data)
    {
        width = data.width;
        height = data.height;
        channels = data.channels;
        pixels = new float[width * height * channels];
        return *this;
    }

    Data(){ pixels = NULL;}
    ~Data()
    {
        if(pixels != NULL)
        delete[] pixels; 
    }
}ImageData;

class ImageIO
{
public:
    static ImageData* readImage(char* filename);//read the image information into a structure
    static bool writeData(char* filename, ImageData* originalImageData);
};

#endif











