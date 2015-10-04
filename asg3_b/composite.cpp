/*************************************************************************
	> File Name: read.cpp
	> Author: Maoting Ren 
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 01:05:55 AM EDT
 ************************************************************************/

#include <iostream>
#include <GL/glut.h>
#include "read.h"
#include "write.h"
#include "manipulation.h"
#include <string.h>

using namespace std;

typedef unsigned int UINT;
Read                *read = NULL;
Write               *write = NULL;
Manipulation        *manipulation = NULL;
char*               writeFilename = NULL;
Write::ImageData*   imageData = NULL;
Write::ImageData*   displayImageData = NULL;//display this structure
Write::ImageData*   imageBufferA;
Write::ImageData*   imageBufferB;
int                 windowWidth;
int                 windowHeight;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(displayImageData->channels == 3)
    {
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGB,GL_UNSIGNED_BYTE, displayImageData->pixels);
    }
    else if(displayImageData->channels == 4)
    {
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGBA,GL_UNSIGNED_BYTE, displayImageData->pixels);
    }
    glFlush();
}

void handleKey(unsigned char key, int x, int y)
{
    switch(key){
        case 'w':
        case 'W':
            write->writeData(writeFilename, manipulation->verticalFlip(displayImageData));
        case 'q':               // q - quit
        case 'Q':
        case 27:                // esc - quit
            exit(0);
        default:                // not a valid key -- just ignore it
            break;
    }
}

void init(int argc, char* argv[])
{
    //this function will read initial variable and read images into imageBuffer
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [A.IMG] " << " [B.IMG]" << std::endl;
        exit(1);
    }
    if(argc < 3)
    {
        writeFilename = new char[100];
        strcpy(writeFilename, "out.png");
    }
    else
        writeFilename = argv[argc - 1];
    read = new Read();
    write = new Write();
    manipulation = new Manipulation();
    imageBufferA = read->readImage(argv[1]);
    imageBufferB = read->readImage(argv[2]);
    imageData = manipulation->composite(imageBufferA, imageBufferB); 
    write->writeData(writeFilename, imageData);
    displayImageData = manipulation->verticalFlip(imageData);
    windowWidth =  imageBufferB->width;
    windowHeight = imageBufferB->height;
}


int main(int argc, char* argv[])
{
    init(argc, argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(500, 200);
    glutCreateWindow("a Simple window");
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKey);
    glBlendEquation(GL_ADD);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    

    glutMainLoop();
    delete displayImageData;
    delete imageBufferA;
    delete imageBufferB;
    return 0;
}


