/*************************************************************************
	> File Name: read.cpp
	> Author: Maoting Ren 
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 01:05:55 AM EDT
 ************************************************************************/

#include <iostream>
#include <GL/glut.h>
#include "manipulation.h"
#include "imageIO.h"
#include <string.h>
#include <cmath>
#include <locale>

using namespace std;
typedef unsigned int UINT;

char*               writeFilename = NULL;
char*               readFilename = NULL;
ImageData*          imageData = NULL;
ImageData*          resultData = NULL;
ImageData*          displayImg= NULL;//display this structure
ImageData*          imageBuffer;
int                 windowWidth;
int                 windowHeight;
bool                flag = false;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(displayImg->channels == 1)
        glDrawPixels(displayImg->width, displayImg->height, GL_LUMINANCE, GL_FLOAT, displayImg->pixels);
    if(displayImg->channels == 3)
        glDrawPixels(displayImg->width, displayImg->height, GL_RGB, GL_FLOAT, displayImg->pixels);
    if(displayImg->channels == 4)
        glDrawPixels(displayImg->width, displayImg->height, GL_RGBA, GL_FLOAT, displayImg->pixels);
    glFlush();
}

void handleKey(unsigned char key, int x, int y)
{
    switch(key){
        case 'w':
        case 'W':
            ImageIO::writeData(writeFilename, Manipulation::verticalFlip(displayImg));
            break;
        case 'i':
        case 'I':
            displayImg = Manipulation::verticalFlip(displayImg);
            display();
            break;
        case 'f':
        case 'F':
            displayImg = Manipulation::horizFlip(displayImg);
            display();
            break;
        case 's':
        case 'S':
            flag = !flag;
            if(flag == true)
                displayImg = imageData;
            else
                displayImg = resultData;
            display();
            break;

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
    if(argc < 2)
    {
        cout << "argc is not correct!" << endl;
        exit(-1);
    }

    //read in the input image
    readFilename = argv[1];
    if(argc > 2)
       writeFilename = argv[2]; 
    imageBuffer = ImageIO::readImage(readFilename); 
    imageData = Manipulation::verticalFlip(imageBuffer); 
    //next, build the transformation matrix
    resultData = Manipulation::change(imageData, 5, 20);
    displayImg = resultData;

    windowWidth = displayImg->width;
    windowHeight = displayImg->height;
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
    return 0;
}


