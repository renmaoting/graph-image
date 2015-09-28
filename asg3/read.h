/*************************************************************************
	> File Name: read.cpp
	> Author: Maoting Ren 
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 01:05:55 AM EDT
 ************************************************************************/

#ifndef _READ_H
#define _READ_H

#include <GL/glut.h>
#include "base.h"

class Read:public Base
{
public:
    ImageData* readImage(char* filename);//read the image information into a structure
};
#endif
