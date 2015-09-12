/*************************************************************************
	> File Name: manipulation.h
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:19:53 AM EDT
 ************************************************************************/

#ifndef _MANIPULATION_H
#define _MANIPULATION_H
#include "base.h"
#include <vector>
#include <iostream>
class Manipulation: public Base
{
public:
    Manipulation(){imageData.pixels = NULL;}
    ~Manipulation(){if(imageData.pixels!= NULL) delete[] imageData.pixels; }
    ImageData& herizontalFlip(ImageData* inputImageData);
    ImageData& verticalFlip(ImageData* inputImageData);
    
    //get a single channel image, and 
    //channel = 0 represent R
    //channel = 1 represent G
    //channel = 2 represent B
    ImageData& getSingleChannel(ImageData* inputImageData, int channel);

private:
    bool copyImageData(ImageData* inputImageData);//copy data form inputImageDataput to member variable
    bool arrayToVector(ImageData* inputImageData);//convert pixel data to 3D vector
    bool vectorToArray();//convert 3D vector to pixel array
    

private:
    std::vector<GLubyte> dataVec3;
    std::vector<std::vector<GLubyte> > dataVec2;
    std::vector<std::vector<std::vector<GLubyte> > > dataVec1;
};
#endif
