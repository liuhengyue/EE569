//
//  raw.h
//  569HW2
//
//  Created by LiuHengyue on 10/5/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//

#ifndef raw_h
#define raw_h
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
/*
 * ReadRaw()
 * Function: This function reads in image data from a RAW image file.
 * Input: file path of the image, size of the image.
 * Output: the pointer of the image array.
 */
unsigned char* ReadRaw(const char *filename, const int width,const int length,const int bytesPerPixel);
/*
 * WriteRaw()
 * Function: This function write image data from a in a RAW file.
 * Input: file path one wants to create, image data array pointer, size of the image.
 */
void WriteRaw(const char *path, unsigned char* imagePtr, const int width,const int length,  const int bytesPerPixel);

#endif /* raw_h */
