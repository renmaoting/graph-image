/**********************************************************************
> File Name: manipulation.cpp
	> Author: Maoting Ren
	> Mail: mren@g.clemson.edu
	> Created Time: Wed 02 Sep 2015 03:24:10 AM EDT
 ************************************************************************/

#include "manipulation.h"
#include <string.h>
#include <algorithm>
#include "math.h"
#include <stdlib.h>
#include <fstream>


//vertically flip the image
ImageData* Manipulation::verticalFlip(ImageData* inputImage)
{
    // this function will vertically flip the image
    if(inputImage== NULL)
    {
        std::cout<< "Manipulation::verticalFlip() parameter inputImageData shouldn't be null in Manipulation::verticalFlip!" << std::endl;
        exit(-1);
    }
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;

    int cnt =0;
    for(int i =0; i< imageData->height; i++ )
        for(int j = 0; j< imageData->width * imageData->channels; j++)
            imageData->pixels[cnt++] = inputImage->pixels[(inputImage->height -i - 1)* inputImage->width* inputImage->channels +j];

    return imageData;
}

ImageData* Manipulation::filt(ImageData* inputImage, const std::vector<std::vector<float> >& vecFilt)
{
    if(inputImage == NULL || vecFilt.size() == 0)
    {
        std::cout << "Manipulation::filt inputImage shouldn't be NULL, or vector should not be NULL!" << std::endl;
        exit(0);
    }    
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;

    for(int i =0; i < imageData->height; i++)
    {
        for(int j =0; j < imageData->width; j++)
        {
            for(int k =0; k < imageData->channels; k++)
            {
                float factor=0 ,cnt =0;
                int N = vecFilt.size();
                for(int x = 0; x < N; x++)
                {
                    for(int y = 0; y < N; y++)
                    {
                        factor += vecFilt[N-x-1][N-y-1];
                        int m = i + x - N/2;
                        int n = j + y - N/2;
                        if(m < 0 || n < 0 || m >= imageData->height || n >= imageData->width)// in the boundary, I just set the outside pixel as 0
                            continue;
                        // in this place, I use reflection
                        cnt += vecFilt[N-x-1][N-y-1] * inputImage->pixels[(i + x - N/2)* inputImage->width *
                                inputImage->channels + (j + y - N/2)*inputImage->channels + k];
                    }
                }

                float value = cnt/factor;
                //if(value <= 0) value = 0;// normalization
                //if(value > 1) value = 1;
                imageData->pixels[i* imageData->width * imageData->channels + j* imageData->channels +k] = value;
            }
        }
    }

    return imageData;
}

ImageData* Manipulation::simpleTM(ImageData* inputImage, float gamma)
{
    // this function is simple tone mapping
    if(inputImage == NULL)
    {
        std::cout<< "Manipulation::simpleTM() parameter inputImage shouldn't be null!" << std::endl;
        exit(-1);
    }
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;
    
    for(int i =0; i < imageData->height; i++)
    {
        for(int j =0; j < imageData->width; j++)
        {
            int index = i*imageData->width* imageData->channels + j*imageData->channels;
            // Lw = 1/61.0 * (20.0R + 40.0G + B)
            float Lw = 1/61.0 *(20.0 * inputImage->pixels[index] + 40.0* inputImage->pixels[index+ 1] + inputImage->pixels[index +2]);
            // Ld = exp(log(Ld)), log(Ld) = gamma * log(Lw)
            float Ld = exp(gamma * log(Lw));
            float factor = Ld/Lw;
            for(int k =0; k < inputImage->channels; k++)
            {
                // Cd = Ld/Lw * C
                imageData->pixels[index + k] = factor * inputImage->pixels[index + k]; 
            }
        }
    }  

    return imageData;
}

ImageData* Manipulation::toneMapping(ImageData* inputImage, int flag, int c, int kSize)
{
    // this function is tone mapping with convolution
    if(inputImage == NULL)
    {
        std::cout<< "Manipulation::toneMappingWithC() parameter inputImage shouldn't be null!" << std::endl;
        exit(-1);
    }

    // create the singe channel image log(Lw)
    ImageData* imageLw = new ImageData();;
    *imageLw = *inputImage;
    imageLw->channels = 1;
    delete imageLw->pixels;
    imageLw->pixels = new float[imageLw->width * imageLw->height];

    for(int i =0; i < imageLw->height; i++)
    {
        for(int j =0; j < imageLw->width; j++)
        {
            int index = i*inputImage->width * inputImage->channels + j * inputImage->channels;
            // Lw = 1/61.0 * (20.0R + 40.0G + B)
            float Lw = 1/61.0 *(20.0 * inputImage->pixels[index] + 40.0* inputImage->pixels[index+ 1] + inputImage->pixels[index +2]);
            imageLw->pixels[i*imageLw->width + j] = log(Lw);// the new image's pixel value i log(Lw)
        }
    }

    // read a box filter
    std::vector<std::vector<float> > vecFilt;   
    char* filtName = new char[30];
    strcpy(filtName, "box.filt");
    ImageIO::readFilter(vecFilt, filtName); 

    // B = log(Lw) across g 
    ImageData* imageB; 
    if(flag == 0)
    {
        // read a box filter
        std::vector<std::vector<float> > vecFilt;   
        char* filtName = new char[30];
        strcpy(filtName, "box.filt");
        ImageIO::readFilter(vecFilt, filtName); 
        imageB = filt(imageLw, vecFilt); 
    }
    else
    {
        imageB = biFilt(imageLw, kSize);
    } 
    
    float minB= 999999,  maxB = -999999;
    for(int i =0; i < imageB->height; i++)
    {
        for(int j =0; j < imageB->width; j++)
        {
            // get the max(B) and min(B)
           if(imageB->pixels[i* imageB->width + j] > maxB) 
                maxB = imageB->pixels[i* imageB->width + j]; 
           if(imageB->pixels[i* imageB->width + j] < minB) 
                minB = imageB->pixels[i* imageB->width + j]; 
        }
    }

    // create a new image S
    ImageData* imageS = new ImageData();
    *imageS = *imageB;
    for(int i =0; i< imageS->height; i++)
    {
        for(int j =0; j< imageS->width; j++)
        {
            int index = i* imageS->width + j;
            // S = log(Lw) - B
            imageS->pixels[index] = imageLw->pixels[index] - imageB->pixels[index];
        }
    } 

    // create a new image Ld
    ImageData* imageLd= new ImageData();
    *imageLd = *imageS;
    float gamma = log(c)/(maxB - minB);
    std::cout<< " gamma = " << gamma << std::endl;
    for(int i =0; i < imageLd->height; i++)
    {
        for(int j =0; j< imageLd->width; j++)
        {
            int index = i*imageLd->width + j;
            // get log(Ld) by equation log(Ld) = gamma * B + S
            float logLd = gamma* imageB->pixels[index] + imageS->pixels[index];
            imageLd->pixels[index] = exp(logLd);// save the result of exp(log(Ld))
        }
    }

    // create the result image Cd
    ImageData* imageCd = new ImageData();
    *imageCd = *inputImage;

    for(int i =0; i< imageCd->height; i++)
    {
        for(int j =0; j< imageCd->width; j++)
        {
            // get the Ld/Lw
            float factor = imageLd->pixels[i* imageLd->width + j] / exp(imageLw->pixels[i* imageLw->width + j]);
            for(int k =0; k < imageCd->channels; k++)
            {
                // Cd  = (Ld/Lw) * C
                imageCd->pixels[i*imageCd->width * imageCd->channels +j*imageCd->channels+ k] =
                    factor* inputImage->pixels[i*inputImage->width * inputImage->channels + j* inputImage->channels + k];
            }
        }
    }

    std::cout << "succed!" << std::endl;
    return imageCd;
}

ImageData* Manipulation::biFilt(ImageData* inputImage, int kSize)
{
    if(inputImage == NULL )
    {
        std::cout << "Manipulation::filt inputImage shouldn't be NULL!" << std::endl;
        exit(0);
    }    
    ImageData* imageData = new ImageData();
    *imageData = *inputImage;

    // create Gausian filter
    std::vector<std::vector<float> > vecFilt;
    float sigma = (kSize/2 -1)* 0.3 + 0.8;
    for(int i = 0; i < kSize; i++)
    {
        std::vector<float> tem;
        for(int j =0; j < kSize; j++)
        {
            // Gaussian filter
            float value = 1/(2*3.14 * sigma*sigma) * exp(-((i - kSize/2 ) *(i - kSize/2 )
                        + (j - kSize/2) * (j -kSize/2 ) ))/(2*sigma*sigma);  
            tem.push_back(value);
        }
        vecFilt.push_back(tem);
    }

    for(int i =0; i < imageData->height; i++)
    {
        for(int j =0; j < imageData->width; j++)
        {
            for(int k =0; k < imageData->channels; k++)
            {
                float factor=0 ,cnt =0;
                int N = vecFilt.size();
                for(int x = 0; x < N; x++)
                {
                    for(int y = 0; y < N; y++)
                    {
                        // deal with the boundary pixels
                        int m =abs( i + x - N/2 );
                        int n =abs(j + y - N/2);
                        if(m >= imageData->height )
                            m = 2*imageData->height - m;
                        if(n >= imageData->width )
                            n = 2*imageData->width - n;

                        // get d 
                        float d =inputImage->pixels[m* inputImage->width *inputImage->channels + n*inputImage->channels + k]
                            -inputImage->pixels[i* inputImage->width * inputImage->channels + j*inputImage->channels + k]; 
                        
                        // w = exp(-clamp(d*d); 
                        float t = d*d;
                        if(t >1) t = 1;
                        float w = exp(-t);
                        factor +=  w* vecFilt[N-x-1][N-y-1];

                        // in this place, I use reflection
                        cnt += w* vecFilt[N-x-1][N-y-1] * inputImage->pixels[m* inputImage->width *
                                inputImage->channels + n*inputImage->channels + k];
                    }
                }

                float value = cnt/factor;
                //if(value <= 0) value = 0;// normalization
                //if(value > 1) value = 1;
                imageData->pixels[i* imageData->width * imageData->channels + j* imageData->channels +k] = value;
            }
        }
    }

    return imageData;
}


ImageData* Manipulation::globalOpt(ImageData* inputImage , float a)
{
    if(inputImage == NULL )
    {
        std::cout << "Manipulation::globalOpt inputImage shouldn't be NULL!" << std::endl;
        exit(0);
    }    
    
    std::vector<std::vector<float> > LwVec;
    std::vector<std::vector<float> > LmVec;
    std::vector<std::vector<float> > LdVec;

    std::vector<float> tem;
    tem.resize(inputImage->width);
    for(int i =0; i< inputImage->height ; i++)
    {
        LwVec.push_back(tem); 
        LmVec.push_back(tem); 
        LdVec.push_back(tem); 
    }

    float Lwhite = 0;
    float sum = 0;
    float LwAverage = 0;

    for(int i =0; i < inputImage->height; i++)
    {
        for(int j =0; j < inputImage->width; j++)
        {
            int index = i*inputImage->width * inputImage->channels + j * inputImage->channels;
            // Lw = 1/61.0 * (20.0R + 40.0G + B)
            float Lw = 1/61.0 *(20.0 * inputImage->pixels[index] + 40.0* inputImage->pixels[index+ 1] + inputImage->pixels[index +2]);
            if(Lw > Lwhite) Lwhite = Lw;
            LwVec[i][j] = Lw;// the new image's pixel value i log(Lw)
            sum += log(0.0001 + Lw);
        }
    }

    std::cout << " Lwhite = "<< Lwhite << std::endl;
    LwAverage = exp(sum / (inputImage->width * inputImage->height));
    for(int i =0; i < inputImage->height; i++)
        for(int j =0; j < inputImage->width; j++)
            LmVec[i][j] = a/LwAverage  * LwVec[i][j];
 
    for(int i =0; i < inputImage->height; i++)
        for(int j =0; j < inputImage->width; j++)
            LdVec[i][j] = LmVec[i][j] * (1 + LmVec[i][j]/(Lwhite * Lwhite))/( 1 + LmVec[i][j]);    

    // create the result image Cd
    ImageData* imageCd = new ImageData();
    *imageCd = *inputImage;

    for(int i =0; i< imageCd->height; i++)
    {
        for(int j =0; j< imageCd->width; j++)
        {
            // get the Ld/Lw
            float factor = LdVec[i][j]  / LwVec[i][j];
            for(int k =0; k < imageCd->channels; k++)
            {
                // Cd  = (Ld/Lw) * C
                imageCd->pixels[i*imageCd->width * imageCd->channels +j*imageCd->channels+ k] =
                    factor* inputImage->pixels[i*inputImage->width * inputImage->channels + j* inputImage->channels + k];
            }
        }
    }

    std::cout << " global operator succed! " << std::endl;
    return imageCd;
}




