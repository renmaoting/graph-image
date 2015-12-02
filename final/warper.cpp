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
#include <vector>
#include <cmath>
#include <locale>

using namespace std;
typedef unsigned int UINT;

char*               writeFilename = NULL;
char*               readFilename = NULL;
ImageData*          imageData = NULL;
ImageData*          displayImg= NULL;//display this structure
vector<ImageData*>  imageBuffer;
vector<ImageData*>  resultImg;
vector<ImageData*>  buttonImg;
ImageData*          frameImg;
int                 windowWidth;
int                 windowHeight;
bool                flag = true;
float               ratioX = 1;//the parameter of glPixelZoom, use to scale image
float               ratioY = 1;
int                 currentImage;//the image displaying now
int                 totalImage;//the number of image user provided 
bool                circle = false;
bool                triangle= false;
bool                quadra= false;

void adujstScale()
{
    // window's width < iamge's witdth
    if(displayImg->width > windowWidth-130*ratioX && displayImg->height <= windowHeight)
    {
        ratioX = (windowWidth-130*ratioX)/(float)displayImg->width;
        ratioY = ratioX;
    }
    // window's height < image's height 
    else if(displayImg->width -130*ratioX<= windowWidth && displayImg->height > windowHeight)
    {
        ratioY = windowHeight/(float)displayImg->height;
        ratioX = ratioY;
    }
    // both window's width and height < image size 
    else if(displayImg->width > windowWidth -130 *ratioX && displayImg->height > windowHeight)
    {
        ratioX = (windowWidth-130*ratioX) /(float)displayImg->width;
        ratioY = windowHeight/(float)displayImg->height;
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    adujstScale(); 
    int posDrawX, posDrawY;
    posDrawX = windowWidth - displayImg->width*ratioX - 130*ratioY;
    posDrawY = windowHeight - displayImg->height*ratioY;
    
    ////if the window's width is small than image, then should draw the image in left x = 0
    //if(windowWidth - displayImg->width- 130*ratioX < 0) posDrawX = 0;
    //if(windowHeight - displayImg->height < 0) posDrawY = 0;
    //from where the image should be drawn
    glRasterPos2i(posDrawX/2, posDrawY/2);
    glPixelZoom(ratioX, ratioY);

    if(displayImg->channels == 1)
        glDrawPixels(displayImg->width, displayImg->height, GL_LUMINANCE, GL_FLOAT, displayImg->pixels);
    if(displayImg->channels == 3)
        glDrawPixels(displayImg->width, displayImg->height, GL_RGB, GL_FLOAT, displayImg->pixels);
    if(displayImg->channels == 4)
        glDrawPixels(displayImg->width, displayImg->height, GL_RGBA, GL_FLOAT, displayImg->pixels);

    for(unsigned int i =0; i< 3; i++)
    {
        glRasterPos2i(windowWidth -110*ratioX, windowHeight/2 - 120*ratioY+ ratioY*i*80);
        glDrawPixels(buttonImg[i]->width, buttonImg[i]->height, GL_RGBA, GL_FLOAT, buttonImg[i]->pixels);
    }

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
                displayImg = imageBuffer[currentImage];
            else
            {
                if(resultImg[currentImage] == NULL)
                    resultImg[currentImage] = Manipulation::change(imageBuffer[currentImage], 5, 20);
                displayImg = resultImg[currentImage];
            }
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

void mouseClick(int button, int state, int x, int y)
{
    if(state == 0)
    {
        cout<< "screen x = " << x << " y = " << y << endl;
        x = x - (windowWidth-130*ratioX- displayImg->width*ratioX)/2;//get the pixel row
        y = windowHeight - y - (windowHeight - displayImg->height*ratioY)/2;// get the pixel column
        //if(x < 0 || x >= displayImg->width) x = 0;//when you click a illegal section, set a default pixel
        //if(y < 0 || y >= displayImg->height) y =0;
        cout<< "image x = " << x << " y = " << y << endl;
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
            flag = true;
            currentImage = (currentImage + 1)% totalImage;
            displayImg = imageBuffer[currentImage];
            display();
            break;
        case GLUT_KEY_LEFT:
            flag = true;
            currentImage = (currentImage - 1 + totalImage) %totalImage;
            displayImg = imageBuffer[currentImage];
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
    imageBuffer.resize(totalImage);
    resultImg.resize(totalImage);
    // inital the resultImg
    for(unsigned int i =0; i< resultImg.size(); i++)
        resultImg[i] = NULL;
 
    //read in the input image
    readFilename = argv[1];
    if(argc > 2)
       writeFilename = argv[2]; 

    // read the button image
    char btn1[20] = "circle.png";
    char btn2[20] = "triangle.png";
    char btn3[20] = "quadra.png";
    char frame[20] = "frame.png";
    imageData =Manipulation::verticalFlip(ImageIO::readImage(btn1));
    buttonImg.push_back(imageData);
    imageData =Manipulation::verticalFlip(ImageIO::readImage(btn2));
    buttonImg.push_back(imageData);
    imageData =Manipulation::verticalFlip(ImageIO::readImage(btn3));
    buttonImg.push_back(imageData);
    frameImg =Manipulation::verticalFlip(ImageIO::readImage(frame));

    for(int i = 1; i < argc -1; i++)
    {
        imageData = ImageIO::readImage(argv[i]);
        imageBuffer[i-1] = Manipulation::verticalFlip(imageData);
    }

    //resultImg= Manipulation::change(imageData, 3, 20);
    displayImg = imageBuffer[0];
    windowWidth = 800;
    windowHeight = 600;
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
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    
    glClearColor(0, 0, 0, 0);    

    glutMainLoop();
    return 0;
}

