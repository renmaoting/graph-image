/*************************************************************************
	> File Name: base.h
	> Author: Maoting Ren
	> Mail: rmtustc@gmail.com
	> Created Time: Wed 02 Sep 2015 02:08:58 AM EDT
 ************************************************************************/

#ifndef _BASE_H
#define _BASE_H
#include <GL/glut.h>
#include <string.h>
class Base
{
public:
    typedef struct Data{
        int width;
        int height;
        int channels;
        GLubyte* pixels;
        Data& operator=(const Data& originalData)
        {
            width = originalData.width;
            height = originalData.height;
            channels = originalData.channels;
            memcpy(pixels, originalData.pixels, width*height*channels);
        }
    }ImageData;

public:
    Base(){}
    ImageData imageData;
};
#endif
