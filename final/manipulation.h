/*************************************************************************
	> File Name: manipulation.h
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:19:53 AM EDT
 ************************************************************************/

#ifndef _MANIPULATION_H
#define _MANIPULATION_H
#include <iostream>
#include "imageIO.h"

class Manipulation
{
public:
    static ImageData* verticalFlip(ImageData* inputImage);
    static ImageData* horizFlip(ImageData* inputImage);
    static ImageData* change(ImageData* inputImage, int radius, int intensityLevel);
};
#endif
