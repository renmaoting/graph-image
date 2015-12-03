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

// this struct is a point
typedef struct Point
{
    Point(int X, int Y):x(X), y(Y){}
    int x;
    int y;
}Point;

class Manipulation
{
public:
    static ImageData* verticalFlip(ImageData* inputImage);
    static ImageData* horizFlip(ImageData* inputImage);
    static ImageData* change(ImageData* inputImage, int radius, int intensityLevel, const std::vector<Point*>& pVec, int buttonType);

private:
    static inline bool checkQuadra(const std::vector<Point*>& pVec, int x, int y);
    //static inline bool checkTriangle(const std::vector<Point*>& pVec, int x, int y);
    static inline bool checkCircle(const std::vector<Point*>& pVec, int x, int y);
    static inline int cross(const Point* a, const Point* b, const Point* p){
        return (b->x - a->x)*(p->y - a->y) - (b->y - a->y)*(p->x - a->x);
    }
    static inline int toLeft(const Point* a, const Point* b, const Point* p) { return cross(a, b, p) > 0;}
    static int checkTriangle(const std::vector<Point*>& pVec,int x, int y);
};
#endif
