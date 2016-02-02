//
//  raw.h
//
//  Created by LiuHengyue on 10/5/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//

#ifndef raw_h
#define raw_h
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
using namespace std;
using namespace cv;

/* imageData declaration */

class imageData{
    
private:
    
public:
    //image related parameters
    unsigned char*** _dataPtr;//store the pixel values
    int _width;
    int _length;
    int _bytesPerPixel;
    //default constructor
    imageData();
    //empty imageData
    imageData(const int width,const int length,const int bytesPerPixel);
    //read image from the file
    imageData(const char* filePath,const int width,const int length,const int bytesPerPixel);
    ~imageData();
    //create empty imageData from current parameters
    imageData createData();
    //create a copied imageData from current imageData
    imageData copy();

    
    
};

/*************************************************************************
 * ReadRaw()
 * Function: This function reads in image data from a RAW image file.
 * Input: file path of the image, size of the image.
 * Output: the pointer of the image array.
 *************************************************************************/

unsigned char* ReadRaw(const char *filename, const int width,const int length,const int bytesPerPixel);

/*************************************************************************
 * WriteRaw()
 * Function: This function write image data from a in a RAW file.
 * Input: file path one wants to create, image data array pointer, size of the image.
 *************************************************************************/

void WriteRaw(const char *path, unsigned char* imagePtr, const int width,const int length,  const int bytesPerPixel);

/*************************************************************************
 * ShowImg()
 * Function: This function reads the image data as vector format, and use imshow() to display the image.
 * Input: ImageData.
 * Output: None.
 *************************************************************************/

void ShowImg(const char* name,imageData data);
#endif /* raw_h */
