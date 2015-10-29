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
#include <vector>

class Manipulation
{
public:
    ImageData* verticalFlip(ImageData* inputImage);
    ImageData* filt(ImageData* inputImage, const std::vector<std::vector<float> >& vecFilt);
    ImageData* simpleTM(ImageData* inputImage, float gamma);
    ImageData* toneMapping(ImageData* inputImage,int flag, int c, int kSize);
    ImageData* biFilt(ImageData* inputImage, int kSize);
    ImageData* BF(ImageData* inputImage, const std::vector<std::vector<float> >& vecFilt);
};
#endif
