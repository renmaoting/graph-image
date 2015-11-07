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

/* 
   Multiply M by a rotation matrix of angle theta
*/
void Rotate(Matrix3x3 &M, float theta){
   int row, col;
   Matrix3x3 R(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   double rad, c, s;

   rad = PI * theta / 180.0;
   c = cos(rad);
   s = sin(rad);

   R[0][0] = c;
   R[0][1] = -s;
   R[1][0] = s;
   R[1][1] = c;

   Matrix3x3 Prod = R * M;

   for(row = 0; row < 3; row++) {
        for(col = 0; col < 3; col++) {
            M[row][col] = Prod[row][col];
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
        case 's':
        case 'S':
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
            break;
        case GLUT_KEY_DOWN:

            break;
        default:
            break;
    }
}



void process_input(Matrix3x3 &M){
    char command[1024];
    bool done;
    float theta;

   /* build identity matrix */
    M.identity();

    for(done = false; !done;) {
        /* prompt and accept input, converting text to lower case */
        printf("> ");
        scanf("%s", command);
        lowercase(command);

        /* parse the input command, and read parameters as needed */
        if(strcmp(command, "d") == 0) {
            done = true;
        }
        else if(strlen(command) != 1) {
            printf("invalid command, enter r, s, t, h, d\n");
        }
        else {
            switch(command[0]) {
                case 'r':		/* Rotation, accept angle in degrees */
                    if(scanf("%f", &theta) == 1)
                        Rotate(M, theta);
                    else
                        fprintf(stderr, "invalid rotation angle\n");
                    break;
                case 's':		/* Scale, accept scale factors */
                    break;
                case 't':		/* Translation, accept translations */
                    break;
                case 'h':		/* Shear, accept shear factors */
                    break;
                case 'd':		/* Done, that's all for now */
                    done = true;
                    break;
                default:
                    printf("invalid command, enter r, s, t, h, d\n");
                    break;
            }
        }
    }
}

void init(int argc, char* argv[])
{
    //this function will read initial variable and read images into imageBuffer
    Matrix3x3 M(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

    //read in the input image
    //

    //next, build the transformation matrix
    process_input(M);

    //cout << "Accumulated Matrix: " << endl;
    //cout << M << endl;

   
    
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


