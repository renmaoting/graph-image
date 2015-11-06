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
#include "Matrix.h"

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
ImageData*          toneMappedImage;
int                 windowWidth;
int                 windowHeight;
float               Gamma = 0.5;
int                 C = 70;
int                 K = 21;
float               A = 0.78;
bool                flagG = false; // if the command contain '-g'
bool                flagA = false; // if the command contain '-g'
bool                flagB = false; // if the command contain '-b'
bool                flagC = false; // if the command contain '-c'
int                 flagS = 1; // indicate original image or tone mapped image
std::vector<std::vector<double> > vecFilt;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(displayImageData->channels == 1)
        glDrawPixels(displayImageData->width, displayImageData->height, GL_LUMINANCE, GL_FLOAT, displayImageData->pixels);
    if(displayImageData->channels == 3)
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGB, GL_FLOAT, displayImageData->pixels);
    glFlush();
}

void handleKey(unsigned char key, int x, int y)
{
    switch(key){
        case 's':
        case 'S':
            if(flagG || flagC || flagB || flagA)
            {
                flagS = (flagS + 1)%2;// flagS = 0 mean it will display original image
                if(flagS == 0)
                    displayImageData = manipulation->verticalFlip(imageBuffer);
                else
                    displayImageData = toneMappedImage;
                display();
            }
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

void specialKeyHandle(int key, int x, int y)
{
    //this function will response to left arrow and right arrow and play image one by one
    switch(key){
        case GLUT_KEY_UP:
            if(flagG && Gamma < 1.0)
            {
                Gamma+= 0.01;
                std::cout << "gamma = " <<Gamma << std::endl;
                imageData = manipulation->simpleTM(imageBuffer, Gamma);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
                break;
            }
            if(flagC && C <= 95)
            {
                C += 5;
                std::cout << "C = " << C << std::endl;
                imageData = manipulation->toneMapping(imageBuffer,0, C, K);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
                break;
            }
            if(flagA)
            {
                A += 0.05;
                std::cout << "A = " << A << std::endl;
                imageData = manipulation->globalOpt(imageBuffer,A);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
                break;
            }
            if(flagB && K < 50)
            {
                K += 2;
                std::cout << "K = " << K << std::endl;
                imageData = manipulation->toneMapping(imageBuffer,1,C, K);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
            }
            break;
        case GLUT_KEY_DOWN:
            if(flagG && Gamma >0) 
            {
                Gamma -= 0.01;
                std::cout << "gamma = " << Gamma << std::endl;
                imageData = manipulation->simpleTM(imageBuffer, Gamma);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
                break;
            }
            if(flagC && C >= 10)
            {
                C-= 5;
                std::cout << "C = " << C << std::endl;
                imageData = manipulation->toneMapping(imageBuffer,0, C, K);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
                break;
            }
            if(flagA)
            {
                A -= 0.05;
                std::cout << "A = " << A << std::endl;
                imageData = manipulation->globalOpt(imageBuffer,A);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
                break;
            }

            if(flagB && K > 3)
            {
                K -= 2;
                std::cout << "K = " << K << std::endl;
                imageData = manipulation->toneMapping(imageBuffer,1,C, K);
                toneMappedImage = manipulation->verticalFlip(imageData);
                displayImageData = toneMappedImage;
                display();
            }

            break;
        default:
            break;
    }
}

void init(int argc, char* argv[])
{
    //this function will read initial variable and read images into imageBuffer
    
    if(argc < 2)
    {
        std::cerr << "argc is to incorrect" << std::endl;
        exit(0);
    }

    if(strcmp(argv[1], "-g") == 0)
    {
        flagG = true;
        // if there is a flag -g
        readFilename = argv[3];
        if(argc > 4)
            writeFilename = argv[4];
        Gamma = (float)atof(argv[2]); 
        imageBuffer = imgIO->readImage(readFilename);
        toneMappedImage = manipulation->simpleTM(imageBuffer, Gamma );
    }
    else if(strcmp(argv[1], "-c") == 0)
    {   
        flagC = true;
        // if there is a flag -c
        readFilename = argv[2];
        if(argc >= 4)
            writeFilename = argv[3];
        imageBuffer = imgIO->readImage(readFilename);
        toneMappedImage = manipulation->toneMapping(imageBuffer,0, C, K);
    }
    else if(strcmp(argv[1], "-b") == 0)
    {   
        flagB = true;
        // if there is a flag -b
        readFilename = argv[2];
        if(argc >= 4)
            writeFilename = argv[3];
        imageBuffer = imgIO->readImage(readFilename);
        toneMappedImage = manipulation->toneMapping(imageBuffer,1 ,C, K);
    }
    else if(strcmp(argv[1], "-a") == 0)
    {   
        flagA = true;
        // if there is a flag -o
        readFilename = argv[2];
        if(argc >= 4)
            writeFilename = argv[3];
        imageBuffer = imgIO->readImage(readFilename);
        toneMappedImage = manipulation->globalOpt(imageBuffer, A);
    }

    else
    {
        readFilename = argv[1];
        if(argc >= 3)
            writeFilename = argv[2];
        imageBuffer = imgIO->readImage(readFilename);
        toneMappedImage = imageBuffer;
    }
    toneMappedImage = manipulation->verticalFlip(toneMappedImage);
    
    displayImageData = toneMappedImage;
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
    glutSpecialFunc(specialKeyHandle);//response to left arrow and right arrow
    glBlendEquation(GL_ADD);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    

    glutMainLoop();
    return 0;
}


