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
#include "vecmat/Matrix.h"

using namespace std;

typedef unsigned int UINT;
char*               writeFilename = NULL;
char*               readFilename = NULL;
ImageData*          imageData = NULL;
ImageData*          displayImageData = NULL;//display this structure
ImageData*          imageBuffer;
int                 windowWidth;
int                 windowHeight;

/*
   Convert the string s to lower case
*/
void lowercase(char *s){
   int i;
   if(s != NULL) {
        for(i = 0; s[i] != '\0'; i++) {
            if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] += ('a' - 'A');
        }
    }
}

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
        case 'w':
        case 'W':
            ImageIO::writeData(writeFilename, Manipulation::verticalFlip(displayImageData));
            break;
        case 'q':               // q - quit
        case 'Q':
        case 27:                // esc - quit
            exit(0);
        default:                // not a valid key -- just ignore it
            break;
    }
}

void process_input(Matrix3x3 &M){
    char command[1024];
    bool done;
    float theta, s, cx, cy;
    int sx, sy, dx, dy, shx, shy;

   /* build identity matrix */
    M.identity();

    for(done = false; !done;) {
        /* prompt and accept input, converting text to lower case */
        cout  << "> ";
        cin >> command;
        lowercase(command);

        /* parse the input command, and read parameters as needed */
        if(strcmp(command, "d") == 0) {
            done = true;
        }
        else if(strlen(command) != 1) {
            cout << "invalid command, enter r, s, t, h, d\n";
        }
        else {
            switch(command[0]) {
                case 'r':		/* Rotation, accept angle in degrees */
                    if(cin >> theta)
                        Manipulation::rotate(M, theta);
                    else
                        cout << "invalid rotation angle\n";
                    break;
                case 's':		/* Scale, accept scale factors */
                    if(cin >> sx >> sy)
                        Manipulation::scale(M, sx, sy);
                    else
                        cout << "invalid scale parameter\n";
                    break;
                case 't':		/* Translation, accept translations */
                    if(cin >> dx >> dy)
                        Manipulation::translate(M, dx, dy);
                    else
                        cout << "invalid translate parameter\n";
                    break;
                case 'h':		/* Shear, accept shear factors */
                    if(cin >> shx >> shy)
                        Manipulation::shear(M, dx, dy);
                    else
                        cout << "invalid sheer parameter\n";
                    break;
                case 'd':		/* Done, that's all for now */
                    done = true;
                    imageData =  Manipulation::warper(imageBuffer, M);
                    break;
                case 'n':
                    if(cin >> s >> cx >> cy)
                        imageData = Manipulation::twirl(imageBuffer, s, cx, cy);
                    else
                        cout << "invalid twirl parameter\n";
                    break;
                default:
                    cout << "invalid command, enter r, s, t, h, d, n\n";
                    break;
            }
        }
    }
}

void init(int argc, char* argv[])
{
    if(argc < 2)
    {
        cout << "argc is not correct!" << endl;
        exit(-1);
    }
    //this function will read initial variable and read images into imageBuffer
    Matrix3x3 M(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

    //read in the input image
    readFilename = argv[1];
    if(argc > 2)
       writeFilename = argv[2]; 
    imageBuffer = ImageIO::readImage(readFilename); 
    //next, build the transformation matrix
    process_input(M);

    //cout << "Accumulated Matrix: " << endl;
    for(int i =0; i< 3; i++)
    {
        for(int j =0; j< 3; j++)
        {
            cout << M[i][j] << " ";
        }
        cout << endl; 
    }

    displayImageData = Manipulation::verticalFlip(imageData); 
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


