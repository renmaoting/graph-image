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
    //ImageData* herizontalFlip(ImageData* inputImageData){}
    ImageData* verticalFlip(ImageData* inputImageData);
    //ImageData& getSingleChannel(ImageData* inputImageData, int channel){}
    ImageData* masking(ImageData* inputImageData);
    void RGBtoHSV ( unsigned char r, unsigned char g, unsigned char b, float &h, float &s, float &v );
    void HSVtoRGB ( float h, float s, float v, unsigned char &r, unsigned char &g, unsigned char &b );
    ImageData* pvMsking(ImageData* inputImageData, float& k);
    void ssMsking(ImageData* inputImageData);
    ImageData* compositeByMasking(ImageData* AImage, ImageData* BImage);
};
#endif
