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
using namespace std;

typedef unsigned int UINT;
Read                *read = NULL;
Write               *write = NULL;
Manipulation        *manipulation = NULL;
char*               writeFilename = NULL;
Write::ImageData*   imageData = NULL;//grab pixels save in this structure
Write::ImageData*   displayImageData = NULL;//display this structure
int                 currentImage;//the image displaying now
int                 totalImage;//the number of image user provided 
Write::ImageData*   imageBuffer;//read the image user provided into imageBuffer
int                 windowWidth = 800;
int                 windowHeight = 600;
float               ratioX = 1;//the parameter of glPixelZoom, use to scale image
float               ratioY = 1;


void adujstScale()
{
    if(displayImageData->width > windowWidth && displayImageData->height <= windowHeight)
    {
        ratioX = windowWidth/(float)displayImageData->width;
        ratioY = ratioX;
    }
    else if(displayImageData->width <= windowWidth && displayImageData->height > windowHeight)
    {
        ratioY = windowHeight/(float)displayImageData->height;
        ratioX = ratioY;
    }
    else if(displayImageData->width > windowWidth && displayImageData->height > windowHeight)
    {
        ratioX = windowWidth/(float)displayImageData->width;
        ratioY = windowHeight/(float)displayImageData->height;
    }
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    adujstScale(); 
    int posDrawX, posDrawY;
    posDrawX = windowWidth - displayImageData->width;
    posDrawY = windowHeight - displayImageData->height;
    
    //if the window's width is small than image, then should draw the image in left x = 0
    if(windowWidth - displayImageData->width < 0) posDrawX = 0;
    if(windowHeight - displayImageData->height < 0) posDrawY = 0;
    //from where the image should be drawn
    glRasterPos2i(posDrawX/2, posDrawY/2);
    glPixelZoom(ratioX, ratioY);
    //GL_LUMINANCE
    if(displayImageData->channels == 3)
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGB,GL_UNSIGNED_BYTE, displayImageData->pixels);
    else if(displayImageData->channels == 4)
        glDrawPixels(displayImageData->width, displayImageData->height, GL_RGBA,GL_UNSIGNED_BYTE, displayImageData->pixels);
    glFlush();
}


bool grabPixels()
{
    //this function will grab current displaying image pixels 
    if(imageData != NULL)
        delete imageData;
    imageData = new Write::ImageData();
    imageData->width = displayImageData->width;
    imageData->height = displayImageData->height;
    imageData->channels = displayImageData->channels;
    imageData->pixels = new GLubyte[imageData->width * imageData->height * imageData->channels];
    if(imageData->channels == 3)
        glReadPixels((windowWidth- displayImageData->width)/2,(windowHeight - displayImageData->height)/2, imageData->width, imageData->height, GL_RGB, GL_UNSIGNED_BYTE, imageData->pixels);
    else if(imageData->channels == 4)
        glReadPixels((windowWidth- displayImageData->width)/2,(windowHeight - displayImageData->height)/2, imageData->width, imageData->height, GL_RGBA, GL_UNSIGNED_BYTE, imageData->pixels);
    return true;
}


void handleKey(unsigned char key, int x, int y)
{
    switch(key){
        case 'b':
        case 'B':
            displayImageData = &manipulation->getSingleChannel(&imageBuffer[currentImage], 2);
            display();
            break;
        case 'c':               // 'c' - cycle to next color
        case 'C':
            break;
        case 'w':
        case 'W':
            grabPixels();
            write->writeData(writeFilename, &manipulation->verticalFlip(imageData));
            break;
        case 'o':
        case 'O':
            displayImageData = &imageBuffer[currentImage];
            display();
            break;
        case 'f':
        case 'F':
            grabPixels();
            displayImageData = &manipulation->herizontalFlip(imageData);
            display();
            break;
        case 'g':
        case 'G':
            displayImageData = &manipulation->getSingleChannel(&imageBuffer[currentImage], 1);
            display();
            break;
        case 'i':
        case 'I':
            grabPixels();
            displayImageData = &manipulation->verticalFlip(imageData);
            display();
            break;
        case 'q':               // q - quit
        case 'Q':
        case 27:                // esc - quit
            exit(0);
        case 'r':
        case 'R':
            displayImageData = &manipulation->getSingleChannel(&imageBuffer[currentImage], 0);
            display();
            break;
        default:                // not a valid key -- just ignore it
            return;
    }
}


void mouseClick(int button, int state, int x, int y)
{
    if(state == 0)
    {
        grabPixels();
        x = x - (windowWidth- displayImageData->width)/2;//get the pixel row
        y = windowHeight - y - (windowHeight - displayImageData->height)/2;// get the pixel column
        if(x < 0 || x >= displayImageData->width) x = 0;//when you click a illegal section, set a default pixel
        if(y < 0 || y >= displayImageData->height) y =0;
        cout<< "x = " << x << " y = " << y << endl;
        int cnt = y * imageData->width* imageData->channels + x*imageData->channels;
        switch (imageData->channels){
            case 1:
                cout<< "color: " << imageData->pixels[cnt] <<endl; 
                break;
            case 2:
                break;
            case 3:
                cout << "color: " <<(UINT) imageData->pixels[cnt] << " " <<(UINT) imageData->pixels[cnt+1] << " " <<(UINT) imageData->pixels[cnt+2]<<endl;
                break;
            case 4:
                cout <<"color: "<<(UINT)imageData->pixels[cnt]<<" "<<(UINT)imageData->pixels[cnt+1]<<" "<<(UINT)imageData->pixels[cnt+2]<<" "<<(UINT)imageData->pixels[cnt+3]<<endl;
                break;

            default:
                break;
        }    
    }
}


void changeWindowsSize(GLsizei width, GLsizei height)
{
    windowWidth = width;
    windowHeight = height;
}


void specialKeyHandle(int key, int x, int y)
{
    //this function will response to left arrow and right arrow and play image one by one
    if(totalImage < 2)
    {
        return;
    }
    switch(key){
        case GLUT_KEY_RIGHT:
            currentImage = (currentImage + 1)% totalImage;
            displayImageData = &imageBuffer[currentImage];
            display();
            break;
        case GLUT_KEY_LEFT:
            currentImage = (currentImage - 1 + totalImage) %totalImage;
            displayImageData = &imageBuffer[currentImage];
            display();
            break;

        default:
            break;
    }
}


void init(int argc, char* argv[])
{
    //this function will read initial variable and read images into imageBuffer
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [filename] " << std::endl;
        exit(1);
    }

    writeFilename = argv[argc - 1];
    currentImage = 0;
    totalImage = argc - 2;
    imageBuffer = new Write::ImageData[totalImage];
    read = new Read();
    write = new Write();
    manipulation = new Manipulation();
    for(int i = 1; i < argc -1; i++)
    {
        read->readImage(argv[i]);
        imageBuffer[i-1].pixels = new GLubyte[read->imageData.width * read->imageData.height * read->imageData.channels];
        imageBuffer[i-1] = manipulation->verticalFlip(&read->imageData); 
    }
    displayImageData = &imageBuffer[0];//set the first image read to display
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
    glutMouseFunc(mouseClick);// mouseClick
    glutSpecialFunc(specialKeyHandle);//response to left arrow and right arrow
    glutReshapeFunc(changeWindowsSize);//when change window size, will adapt change
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,windowWidth, 0, windowHeight);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    

   glClearColor(1,1,1,1);    

    glutMainLoop();
    return 0;
}


