//
// 
//  EE569 HOMEWORK 4
//  
//  OpenCV is used in problem 3.
//
//  4 C++ files with 3 header files, 7 files totally.
//  
//  Problem 1 involves the codes of thinning processing in homework 3, the codes are 
//  attached here. Raw files manipulations are based on the codes of homework 1, not 
//  attached here either.
//
//  Important:
//  The program requires input file, one may need change the file paths
//  in main() function.
//
//  Created by LiuHengyue on 10/25/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//
// Below are the class and function declarations

//////////////////////////////////////////////////////////
//    class::character
//////////////////////////////////////////////////////////

class character{
private:
public:
    imageData _symbol;
    char _shape;
    int _EulerNo;
    int _strokeCut;
    float _A0;
    float _P0;
    float _xCog;
    float _yCog;
    float _circularity;
    float _aspect;
    float _symmetry;
    
    //default construct
    character(){
        _symbol=imageData();
        _shape=0;
        _EulerNo=0;
        _A0=0;
        _P0=0;
        _xCog=0;
        _yCog=0;
        _circularity=0;
        _aspect=0;
    }
    character(imageData img, int shape,int euler,float a0,float p0,float xCog,float yCog){
        _symbol=img;
        _shape=shape;
        _EulerNo=euler;
        _A0=a0;
        _P0=p0;
        _xCog=xCog;
        _yCog=yCog;
    }
    //set data for each character
    void setSymbol(imageData symbol);
    void setShape(char s);
    bool empty();
    //will implement later
    void setFeatures();
    //display geometric features
    void showFeatures();
    
    void saveFeatures(const char* path);
};
//////////////////////////////////////////////////////////
//    pre-processing functions for OCR system
//////////////////////////////////////////////////////////

/*************************************************************************
 * Color2Gray()
 * Function: This function converts color image into gray-scale image.
 * Input: Original color image.
 * Output: Gray scale image.
 *************************************************************************/

imageData Color2Gray(imageData img);

/*************************************************************************
 * GetHist()
 * Function: This function computes the histogram of the gray-scale image.
 * Input: Gray-scale image.
 * Output: Corresponding histogram.
 *************************************************************************/

int* GetHist(imageData img);

/*************************************************************************
 * BrightnessEnhancement()
 * Function: This function increases the intensity values of original image.
 * Input: Original gray-scale image.
 * Output: Brighter gray-scale image.
 *************************************************************************/

imageData BrightnessEnhancement(imageData img);

/*************************************************************************
 * TransferFunctionEqualization()
 * Function: This function calculates the transfer function of the image,
 *           and apply the equalization.
 * Input: Original gray-scale image.
 * Output: Equalized gray-scale image.
 *************************************************************************/

imageData TransferFunctionEqualization(imageData img);

/*************************************************************************
 * Binarization()
 * Function: This function converts gray-scale image into binary image.
 * Input: Original gray-scale image.
 * Output: Binary black/white image.
 *************************************************************************/

imageData Binarization(imageData img);

/*************************************************************************
 * PreProcess()
 * Function: This function is the pre-process step of OCR system. It applies
 *           these steps from above functions:
 *              1. Transform color image to gray-scale image.
 *              2. Enhance the brightness.
 *              3. Histogram equalization.
 *              4. Binarization.
 * Input: Original color image.
 * Output: Binary image.
 *************************************************************************/

imageData PreProcess(imageData img);

//////////////////////////////////////////////////////////
//    segmentation of symbols
//////////////////////////////////////////////////////////

/*************************************************************************
 * InVector()
 * Function: This function tells if a certain label is in certain label
 *           equivalence table.
 * Input: label equivalence table, particular label.
 * Output: true or false.
 *************************************************************************/

bool InVector(int label,vector<int> list);

/*************************************************************************
 * MinLabel()
 * Function: This function generates the smallest label for certain table.
 * Input: certain label equivalence table.
 * Output: smallest label.
 *************************************************************************/

int MinLabel(vector<int> list);

/*************************************************************************
 * BBSymbol()
 * Function: This function generates particular symbol with corresponding
 *           bounding box.
 * Input: Labeled image, particular label, original image width and length.
 * Output: One symbol.
 *************************************************************************/

character BBSymbol(int** labels,int label, int width, int length);

/*************************************************************************
 * SymbolSegment()
 * Function: This function first label the image into several segmentations
 *           using the connected component labeling method provided in
 *           discussion 9; then find each symbol's ROI and extract each
 *           symbol with bounding box.
 * Input: Binary image(after pre-processing).
 * Output: A set of symbols.
 *************************************************************************/

vector<character> SymbolSegment(imageData img);

/*************************************************************************
 * SpatialMoment()
 * Function: This function calculates the scaled spatial central moment of
 *           each symbol.
 * Input: symbol image, horizontal order m, vertical order n, Centroid x,
 *        Centroid y.
 * Output: Float Moment value.
 *************************************************************************/

float SpatialMoment(imageData img, int m, int n,float xCog,float yCog);

/*************************************************************************
 * StrokeCut()
 * Function: This function counts the number of the cuts of one horizontal
 *           line and one vertical line.
 * Input: one character (candidate).
 * Output: number of cuts.
 *************************************************************************/

int StrokeCut(imageData sample);

/*************************************************************************
 * Symmetry()
 * Function: This function computes the symmetry of the character.
 * Input: one character (candidate).
 * Output: symmetry rate (0-1).
 *************************************************************************/

float Symmetry(imageData sample);

/*************************************************************************
 * setFeatures()
 * Function: This function checks each Bit Quad 4*4 pixels through the
 *           symbol image, then calculates the Area and Perimeter geometric
 *           features of the symbol; then computes the scaled spatial central
 *           moment.
 * Input: Symbol image.
 * Output: Related geometric symbols.
 *************************************************************************/
//void character::setFeatures();

/*************************************************************************
 * OCRDecisionTree()
 * Function: This function builts the decision tree. Read the test image's
 *           features and make decisions.
 * Input: Symbol image.
 * Output: prediction of test character.
 *************************************************************************/

char OCRDecisionTree(character candidate);

/*************************************************************************
 * Training()
 * Function: This function read the training images and calculate the features;
 * then, save the features to a path.
 * Input: path to save features.
 * Output: None. *************************************************************************/

void Training(const char* trainPath);

/*************************************************************************
 * Testing()
 * Function: This function read the testing images and calculate the features;
 * then, save the features to a path. Go through the decision tree and make
 * predictions.
 * Input: path to save features.
 * Output: None. *************************************************************************/

void Testing(imageData testImg,const char* testPath);

