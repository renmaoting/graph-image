/*************************************************************************
	> File Name: base.h
	> Author: Maoting Ren
	> Mail: rmtustc@gmail.com
	> Created Time: Wed 02 Sep 2015 02:08:58 AM EDT
 ************************************************************************/

#ifndef _BASE_H
#define _BASE_H
#include <GL/glut.h>
class Base
{
public:
    typedef struct Data{
        int width;
        int height;
        int channels;
        GLubyte* pixels;
        Data(){ pixels = NULL;}
        ~Data()
        {
            if(pixels != NULL)
                delete[] pixels; 
        }
    }ImageData;

public:
    Base(){}
};
#endif
