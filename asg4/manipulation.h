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
    ImageData* verticalFlip(ImageData* inputImageData);
    ImageData* filt(ImageData* inputImage, const std::vector<std::vector<double> >& vecFilt);
    bool gabor(std::vector<std::vector<double> >& vecFilt, int theta, double sigma, double period);
};
#endif
