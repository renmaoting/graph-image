/*************************************************************************
	> File Name: imageIO.h
	> Author: Maoting Ren
	> Mail: rmtustc@gmail.com
	> Created Time: Sun 04 Oct 2015 12:11:58 PM EDT
 ************************************************************************/
#ifndef IMAGEIO_H
#define IMAGEIO_H

#include <GL/glut.h>
#include <vector>

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


class ImageIO
{
public:
    ImageData* readImage(char* filename);//read the image information into a structure
    bool writeData(char* filename, ImageData* originalImageData);
    bool readFilt(std::vector<std::vector<double> >& vecFilt, char* filename);
};

#endif











