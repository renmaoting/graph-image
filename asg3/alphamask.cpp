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
Write::ImageData*   imageBuffer;
int                 windowWidth;
int                 windowHeight;
float               k = 1.0;
bool pv = false, ss = false; 

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //GL_LUMINANCE
    if(displayImageData->channels == 3)
    {
        std::cout <<"channels: " << displayImageData->channels << std::endl;
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGB,GL_UNSIGNED_BYTE, displayImageData->pixels);
    }
    else if(displayImageData->channels == 4)
    {
        std::cout <<"channels: " << displayImageData->channels << std::endl;
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

void specialKeyHandle(int key, int x, int y)
{
    //this function will response to left arrow and right arrow and play image one by one
    switch(key){
        case GLUT_KEY_UP:
            if(k < 5)
                k += 0.1;
            std::cout << "k = " << k << std::endl;
            displayImageData = manipulation->verticalFlip(imageBuffer);
            manipulation->pvMsking(displayImageData, k);
            display();
            break;
        case GLUT_KEY_DOWN:
            if(k > 0.5)
                k -= 0.1;
            std::cout << "k = " << k << std::endl;
            manipulation->pvMsking(displayImageData, k);
            display();
        default:
            break;
    }
}


void init(int argc, char* argv[])
{
    std::cout << "argc = " << argc << std::endl;
    for(int i =0; i< argc ; i++)
        std::cout << "argv " << i <<" = " << argv[i] << std::endl; 

    //this function will read initial variable and read images into imageBuffer
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [inputfilename] " << " [outputfilename]" << std::endl;
        exit(1);
    }
    writeFilename = argv[argc - 1];
    std::cout << "writeFilename = " << writeFilename << std::endl;
    read = new Read();
    write = new Write();
    manipulation = new Manipulation();
   
    //if(strcmp(argv[1], "-pv") == 0 || strcmp(argv[2], "-pv") == 0) pv = true;
    //if(strcmp(argv[1], "-ss") == 0 || strcmp(argv[2], "-ss") == 0) ss = true;
    //std::cout << "pv = " << pv << " ss = " << ss << std::endl;
    //if(pv == true && ss == true)
    //{
    //    if(argc < 5)
    //    {
    //        std::cerr << "error usage" << std::endl;
    //        exit(-1);
    //    }
    //    imageBuffer = read->readImage(argv[3]);
    //    imageData = manipulation->pvMsking(imageBuffer, k);// imageData;
    //    manipulation->ssMsking(imageData);// imageData;
    //    write->writeData(writeFilename, imageData);
    //    imageBuffer = read->readImage(writeFilename);
    //}
    
    //else if(pv == true)
    //{
    //    if(argc < 4)
    //    {
    //        std::cerr << "error usage" << std::endl;
    //        exit(-1);
    //    }
    //    imageBuffer = read->readImage(argv[2]);
    //    imageData = manipulation->pvMsking(imageBuffer, k);// imageData;
    //    write->writeData(writeFilename, imageData);
    //    imageBuffer = read->readImage(writeFilename);
    //}
    //else if(ss == true)
    //{
    //     if(argc < 4)
    //    {
    //        std::cerr << "error usage" << std::endl;
    //        exit(-1);
    //    }
    //    imageBuffer = read->readImage(argv[2]);
    //    imageData = manipulation->masking(imageBuffer); 
    //    manipulation->ssMsking(imageData);// imageData;
    //    write->writeData(writeFilename, imageData);
    //    imageBuffer = read->readImage(writeFilename);
    //}
    //else
    //{
    //   // imageBuffer = read->readImage(argv[1]);
    //    //imageData = manipulation->masking(imageBuffer);// imageData);
    //   // write->writeData(writeFilename, imageData);
    //   // imageBuffer = read->readImage(writeFilename);
    //}

    imageBuffer = read->readImage(argv[1]);
    imageData = manipulation->masking(imageBuffer);// imageData);
    write->writeData(writeFilename, imageData);
    imageBuffer = read->readImage(writeFilename);
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
    glutSpecialFunc(specialKeyHandle);//response to left arrow and right arrow
    glutKeyboardFunc(handleKey);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    

    glutMainLoop();
    delete displayImageData;
    delete imageBuffer;
    return 0;
}


