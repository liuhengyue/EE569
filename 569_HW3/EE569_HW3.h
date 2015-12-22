//
//  main.h
//  569_HW3
//  This is the header file for homework 3
//  Note: c++ class and vector are utilized in this assignment;
//        for classes, private data structure is not defined for less functions and
//        convenience of invoking.
//  Created by LiuHengyue on 10/25/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//

#ifndef main_h
#define main_h
#include <iostream>
#include <math.h>
//raw operations
#include "raw.h"
using namespace cv;
using namespace std;

/*************************************************************************
 *surface declaration
 * This class contains the image intensity information, as well as the world coordinates
 *************************************************************************/
class surface{
private:
public:
    imageData _imageData;
    //store the world coordinates
    float*** _coordinates;
    //unit length of world coordinates
    float _unit;
    surface(int face, imageData data, float unit=0.01);
    //display the color intensities and coordinates
    void showInfo();
    ~surface();
};
/*************************************************************************
 *cube declaration
 * This class contains 6 surfaces
 *************************************************************************/
class cube{
private:
public:
    //contain 6 surfaces
    vector<surface> _surface;
    //store particular surface in the vector
    void setSurface(surface s);
    //project the cube to the image plane
    //r is the vector from orgin of world coordinates to the origin of the camera coordinates; f is the focal length.
    imageData prjectCube(float r[3],float f,float density);
    
};

/*************************************************************************
 * Swirl()
 * Function: This function reads the image data and do the swirl effect.
 * Input: ImageData; Angle; center coordinates
 * Output: swirled imageData.
 *************************************************************************/

imageData Swirl(imageData img, float angle,pair<int, int> center);

/*************************************************************************
 * IndexMatrix()
 * Function: This function generates a N by N Bayer index matrix. Prepare
 for the threshold matrix generation.
 * Input: dimension N.
 * Output: N*N index matrix.
 *************************************************************************/
int* IndexMatrix(int index);

/*************************************************************************
 * ThresholdMatrix()
 * Function: This function generates a N by N threshold matrix. Prepare
 for the dithering process.
 * Input: dimension N.
 * Output: N*N threshold matrix.
 *************************************************************************/

float* ThresholdMatrix(int index);

/*************************************************************************
 * BayerDither()
 * Function: This function reads the image data and do the dithering approach
 to change the image into binary image with threshold matrix.
 * Input: ImageData; threshold matrix dimension.
 * Output: dithered imageData.
 *************************************************************************/

imageData BayerDither(imageData img,int index);

/*************************************************************************
 * FourLevelDither()
 * Function: This function reads the image data and do the dithering approach
 to change the image into 4-gray-level image with threshold matrix.
 * Input: ImageData; threshold matrix.
 * Output: dithered imageData.
 *************************************************************************/

imageData FourLevelDither(imageData img,int index);

/*************************************************************************
 * ErrorDiffusion()
 * Function: This function reads the image data and do the dithering approach
 to change the image into binary image with error diffusion methods.
 * Input: ImageData; error diffusion matrix type.
 * Output: dithered imageData.
 *************************************************************************/

imageData ErrorDiffusion(imageData img,int type);

/*************************************************************************
 * MBVQ()
 * Function: This function reads the rgb pixel value and return its MBVQ 4
 vertexes.
 * Input: pixel value array.
 * Output: MBVQ 4 vertexes.
 *************************************************************************/
int** MBVQ(unsigned char* rgb);

/*************************************************************************
 * ColorQuantize()
 * Function: This function reads the cmy pixel value and its MBVQ, then
             calculate the nearest distance to a certain vertex V. Then
             record the vertex values.
 * Input: pixel CMY value array; its MBVQ.
 * Output: index of the vertex.
 *************************************************************************/
int ColorQuantize(float* cmy, int** mbvq);

/*************************************************************************
 * VectorColorHalftoning()
 * Function: This function reads the image data and do the dithering approach
             to change the color image into halftoning image via vector color
             approach and FS error diffusion.
 * Input: ImageData.
 * Output: dithered imageData.
 *************************************************************************/
imageData VectorColorHalftoning(imageData img);

/*************************************************************************
 * HitOrMiss()
 * Function: This function reads the normalized image data and apply
             the hit or miss filters.
 * Input: ImageData; morphological filters I and II.
 * Output: morphological imageData.
 *************************************************************************/
imageData HitOrMiss(imageData img, int numI, int filterI[][8], int numII, int filterII[][8]);

/*************************************************************************
 * Morphing()
 * Function: This function reads the image data and apply the hit or miss
             filters to achieve shrinking/ thining / skeletonizing.
 * Input: ImageData; type for shrinking, thinning or skeletonizing;
 int inverse: inverse the color of image (default 0).
 * Output: morphological imageData.
 *************************************************************************/
imageData Morphing(imageData img, int type, int inverse);


/*************************************************************************
 * HoleFilling()
 * Function: This function reads the image data and apply hole-filling
             filter to the image.
 * Input: ImageData; shrinked holes locations; if inverse=1 ,reverse 0&1
 * Output: hole-filled imageData./
 *************************************************************************/

imageData HoleFilling(imageData img,bool inverse);

/*************************************************************************
 * BoundarySmooth()
 * Function: This function reads the image data and apply boundary smoothing
             filter to the image.
 * Input: ImageData; shrinked holes locations
 * Output: boundary-smoothed imageData.
 *************************************************************************/

imageData BoundarySmooth(imageData img);

/*************************************************************************
 * ComponentLabeling()
 * Function: This function reads the image data and find the total number of
             objects in the image.
 * Input: ImageData.
 * Output: imageData with labeled with different color.
 *************************************************************************/

imageData ComponentLabeling(imageData img);



#endif /* main_h */
