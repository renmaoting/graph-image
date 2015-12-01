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
    static void shear(Matrix3x3 &M, float shx, float shy); 
    static void scale(Matrix3x3 &M, float sx, float sy);
    static ImageData* twirl(ImageData* inputImage, float s, float cx, float cy, bool flag);
    static void perspective(Matrix3x3 &M, float px, float py);
    static ImageData* warper(ImageData* inputImage, Matrix3x3 &M, bool flag);
    static float bilinear(ImageData* inputImage, Vector3d& vecSrc, int k);
    static float superSampling(ImageData* inputImage, Matrix3x3& M, Vector3d& vecDes, int k, float s, int cx, int cy, int md, bool flag);
    static Vector3d getPespOrigPos(Matrix3x3 &M, Vector3d vecDes);
    static Vector3d getTwirlOrigPos(Vector3d& vecDst, float s, int cx, int cy, int md);
private:
    static ImageData* fwdTransform(ImageData* inputImage, Matrix3x3& M, int& leftMost, int& topMost);
};
#endif
