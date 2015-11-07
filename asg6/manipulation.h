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
#include "Matrix.h"

class Manipulation
{
public:
    static ImageData* verticalFlip(ImageData* inputImage);
    static void translate(Matrix3x3 &M, int dx, int dy);
    static void rotate(Matrix3x3 &M, float theta);
    static void shear(Matrix3x3 &M, int shx, int shy); 
    static void scale(Matrix3x3 &M, int sx, int sy);
    static ImageData* warper(ImageData* inputImage, Matrix3x3 &M);

private:
    static ImageData* fwdTransform(ImageData* inputImage, Matrix3x3 &M);     

};
#endif
