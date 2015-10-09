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

using namespace std;

typedef unsigned int UINT;
Manipulation        *manipulation = NULL;
char*               writeFilename = NULL;
char*               readFilename = NULL;
char*               filtFilename = NULL;
ImageIO*            imgIO = NULL;
ImageData*          imageData = NULL;
ImageData*          displayImageData = NULL;//display this structure
ImageData*          imageBuffer;
int                 windowWidth;
int                 windowHeight;
std::vector<std::vector<double> > vecFilt;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(displayImageData->channels == 1)
        glDrawPixels(displayImageData->width, displayImageData->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, displayImageData->pixels);
    if(displayImageData->channels == 3)
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGB,GL_UNSIGNED_BYTE, displayImageData->pixels);
    else if(displayImageData->channels == 4)
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGBA,GL_UNSIGNED_BYTE, displayImageData->pixels);
    glFlush();
}

void handleKey(unsigned char key, int x, int y)
{
    switch(key){
        case 'c':
            displayImageData = manipulation->filt(displayImageData, vecFilt);
            display();
            break;
        case 'r':
            displayImageData = manipulation->verticalFlip(imageBuffer);
            display();
            break;
        case 'w':
        case 'W':
            imgIO->writeData(writeFilename, manipulation->verticalFlip(displayImageData));
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
    //this function will read initial variable and read images into imageBuffer
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [filtFilename] " << " [inputfilename]" << std::endl;
        exit(1);
    }
    manipulation = new Manipulation();
    imgIO = new ImageIO();
    readFilename = new char[100];
    strcpy(readFilename, "convolve//images//"); 
    if(strcmp(argv[1], "-g") == 0)
    {
        if(argc > 6)
            writeFilename = argv[6];
        strcat(readFilename, argv[5]);
        manipulation->gabor(vecFilt, atoi(argv[2]), atof(argv[3]), atof(argv[4]));
    }

    else
    {
        if(argc > 3)
            writeFilename = argv[3];
        filtFilename = new char[100];
        strcpy(filtFilename, "convolve//filters//"); 
        strcat(filtFilename, argv[1]);
        strcat(readFilename, argv[2]);
        
        imgIO->readFilter(vecFilt, filtFilename);
    }
    imageBuffer = imgIO->readImage(readFilename);
    displayImageData = manipulation->verticalFlip(imageBuffer);

    windowWidth = imageBuffer->width;
    windowHeight = imageBuffer->height;
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


