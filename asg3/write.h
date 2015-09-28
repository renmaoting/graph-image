/*************************************************************************
	> File Name: write.h
	> Author: Maoting Ren 
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 01:57:16 AM EDT
 ************************************************************************/

#ifndef _WRITE_H
#define _WRITE_H

#include "base.h"

class Write:public Base
{
public:
    Write(){}
    ~Write(){}
    bool writeData(char* filename, ImageData* originalImageData);
};

#endif
