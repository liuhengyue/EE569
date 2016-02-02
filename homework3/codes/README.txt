//
// 
//  EE569 HOMEWORK 3
//  
//  OpenCV Mat() and imshow() are used in this assignment just for displaying the
//  results conveniently.
//  
//  Two C++ files with 2 header files, 4 files totally.
//  Need one input argument (1 2 or 3) to specify which problem to run.
//  In running time, input (a b c or d) may needed to specify which sub-problem
//  to run.

//  Important:
//  The program requires input file, one may need change the file paths
//  in main() function.
//
//  Created by LiuHengyue on 10/25/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//
// Below are the class and function declarations

/////////////////////////////////////////////////////////////////
//
//       raw.h
//
/////////////////////////////////////////////////////////////////

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


/////////////////////////////////////////////////////////////////
//
//       EE569_HW3.h
//
/////////////////////////////////////////////////////////////////

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
 * Input: ImageData; shrinked holes locations
 * Output: hole-filled imageData.
 *************************************************************************/

imageData HoleFilling(imageData img, imageData loc);

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

