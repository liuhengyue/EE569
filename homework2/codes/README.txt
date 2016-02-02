//
//  main.cpp
//  569HW2 
//
//  Created by LiuHengyue on 10/1/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//
#ifndef main_h
#define main_h
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <math.h>
#include<iomanip>
//opencv header files
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <iostream>
#include "raw.h"
using namespace std;
using namespace cv;
#endif
/*
 * ReadRaw()
 * Function: This function reads in image data from a RAW image file.
 * Input: file path of the image, size of the image.
 * Output: the pointer of the image array.
 */
unsigned char* ReadRaw(const char *filename, const int width,const int length,const int bytesPerPixel){
    // Allocate image data array using new() for transmiting array pointer
    unsigned char *imagedata;
    imagedata=new unsigned char[width*length*bytesPerPixel];
    FILE *file;
    // Open file
    if (!(file=fopen(filename,"rb"))) {
        cout << "Cannot open file: " << filename <<endl;
        exit(1);
    }
    fread(imagedata, sizeof(unsigned char), width*length*bytesPerPixel, file);
    fclose(file);
    //delete Imagedata; may use later
    return imagedata;//return the address of the first pixel
}

/*
 * WriteRaw()
 * Function: This function write image data from a in a RAW file.
 * Input: file path one wants to create, image data array pointer, size of the image.
 */
void WriteRaw(const char *path, unsigned char* imagePtr, const int width,const int length,  const int bytesPerPixel){
    FILE *file;
    //const char* path="/Users/liuhengyue/Google Drive/EE569/hw2/569HW2/569HW2/straw_01ba.raw";
    if (!(file=fopen(path,"wb"))) {
        cout << "Cannot open file: " << path << endl;
        exit(1);
    }
    unsigned char outImage[length][width][bytesPerPixel];
    int i=0,j=0,dim=0;
    for(dim=0;dim<bytesPerPixel;dim++){
        for(i=0;i<length;i++){//array row number
            for(j=0;j<width;j++){//coloumn number
                outImage[i][j][dim]=imagePtr[j+i*width+dim*width*length];            }
        }
    }
    fwrite(outImage, sizeof(unsigned char), width*length*bytesPerPixel, file);
    fclose(file);
}
    //Other functions wirtten here
    /*
     *Mean calculation and extraction
     
    
    int i=0,j=0,dim=0;
    for(dim=0;dim<bytesPerPixel;dim++){
        for(j=0;j<Length;j++){
            for(i=0;i<Width;i++){
                cout<<Imagedata[i][j][dim]<<endl;
            }
        }
    }
     */

/*
 * ImageExtendBoundary()
 * Function: This function reads in image data, then add 2 rows and 2 columns to the original image,
 *           copy the pixel values axisymmetric about the boundaries.
 * Input: image data array pointer, image size.
 * Output: the pointer of the image array, the image size becomes width+4 * length+4.
 */
unsigned char* ImageExtendBoundary(unsigned char* imagePtr, const int width,const int length,const int bytesPerPixel){
    
    unsigned char originalImage[length][width][bytesPerPixel];
    unsigned char extendedImage[length+4][width+4][bytesPerPixel];
    unsigned char* extendedImagePtr;
    extendedImagePtr=new unsigned char[(width+4)*(length+4)*bytesPerPixel];
    int i=0,j=0,dim=0;
    for(dim=0;dim<bytesPerPixel;dim++){
        for(i=0;i<length;i++){
            for(j=0;j<width;j++){
                originalImage[i][j][dim]=imagePtr[j+i*width+dim*width*length];//assign pixel values from the array pointer
                extendedImage[i+2][j+2][dim]=originalImage[i][j][dim];//copy values to new image
            }
        }
    }
    
    for(dim=0;dim<bytesPerPixel;dim++){
        //fill up left-top 4 pixels
        extendedImage[0][0][dim]=originalImage[0][0][dim];
        extendedImage[1][0][dim]=originalImage[1][0][dim];
        extendedImage[0][1][dim]=originalImage[0][1][dim];
        extendedImage[1][1][dim]=originalImage[1][1][dim];
        //fill up left-bottom 4 pixels
        extendedImage[0][width+3][dim]=originalImage[0][width-1][dim];
        extendedImage[1][width+3][dim]=originalImage[1][width-1][dim];
        extendedImage[0][width+2][dim]=originalImage[0][width-2][dim];
        extendedImage[1][width+2][dim]=originalImage[1][width-2][dim];
        //fill up right-top 4 pixels
        extendedImage[length+3][0][dim]=originalImage[length-1][0][dim];
        extendedImage[length+3][1][dim]=originalImage[length-1][1][dim];
        extendedImage[length+2][0][dim]=originalImage[length-2][0][dim];
        extendedImage[length+2][1][dim]=originalImage[length-2][1][dim];
        //fill up right-bottom 4 pixels
        extendedImage[length+3][width+3][dim]=originalImage[length-1][width-1][dim];
        extendedImage[length+3][width+2][dim]=originalImage[length-1][width-2][dim];
        extendedImage[length+2][width+3][dim]=originalImage[length-2][width-1][dim];
        extendedImage[length+2][width+2][dim]=originalImage[length-2][width-2][dim];
        //fill up left 2 and right 2 columns
        for (i=0; i<length; i++) {
            extendedImage[i+2][0][dim]=originalImage[i][0][dim];
            extendedImage[i+2][1][dim]=originalImage[i][1][dim];
            extendedImage[i+2][width+3][dim]=originalImage[i][width-1][dim];
            extendedImage[i+2][width+2][dim]=originalImage[i][width-2][dim];
        }
        //fill up top 2 and bottom 2 rows
        for(j=0;j<width;j++){
            extendedImage[0][j+2][dim]=originalImage[0][j][dim];
            extendedImage[1][j+2][dim]=originalImage[1][j][dim];
            extendedImage[length+3][j+2][dim]=originalImage[length-1][j][dim];
            extendedImage[length+2][j+2][dim]=originalImage[length-2][j][dim];
        }
        
    }
    
    for(dim=0;dim<bytesPerPixel;dim++){
        for(i=0;i<length+4;i++){
            for(j=0;j<width+4;j++){
                extendedImagePtr[j+i*(width+4)+dim*(width+4)*(length+4)]=extendedImage[i][j][dim];
            }
        }
        
    }
    
    return extendedImagePtr;
    
}
/* 
 * MeanExtract
 * Function: This function reads in image data, then calculates the center pixel's mean values via 5*5
 *           window, then extract the mean value at the center pixel.
 * Input: image data array pointer, image size.
 * Output: the pointer of the image array.
 */
unsigned char* MeanSubtract(unsigned char* imagePtr, const int width,const int length,const int bytesPerPixel){
    unsigned char originalImage[length][width][bytesPerPixel];
    unsigned char extendedImage[length+4][width+4][bytesPerPixel];
    //deal with boundary problem
    unsigned char *extendedImagePtr=ImageExtendBoundary(imagePtr, width, length, bytesPerPixel);
    //store the mean of each pixel
    unsigned char meanImage[length][width][bytesPerPixel];
    //output image data
    unsigned char *subtractedImage=new unsigned char[width*length*bytesPerPixel];
    int i=0,j=0,dim=0;
    for(dim=0;dim<bytesPerPixel;dim++){
        for(i=0;i<length;i++){
            for(j=0;j<width;j++){
                originalImage[i][j][dim]=*(imagePtr+j+i*width+dim*width*length);//assign pixel values from the array pointer
            }
        }
        for(i=0;i<length+4;i++){
            for(j=0;j<width+4;j++){
                extendedImage[i][j][dim]=*(extendedImagePtr+j+i*width+dim*width*length);//assign pixel values from the array pointer
            }
        }

    }
    for(dim=0;dim<bytesPerPixel;dim++){
        for(i=0;i<length;i++){
            for(j=0;j<width;j++){
                meanImage[i][j][dim]=0;
                //using 5x5 window
                for(int wl=0;wl<5;wl++){
                    for(int ww=0;ww<5;ww++){
                    //calculate mean in the window
                    meanImage[i][j][dim]+=extendedImage[i+wl][j+ww][dim]/25;
                    }
                }
                                //subtract mean
                subtractedImage[j+i*width+dim*width*length]=originalImage[i][j][dim]-meanImage[i][j][dim];
                //cout<<int(subtractedImage[j+i*width+dim*width*length])<<endl;

            }
        }
    }
    return subtractedImage;

}
/*
 * FilterBank()
 * Function: This function reads in reduced DC image data after pre-processing; 
 *           then use 25 filters to get 25 output filtered images
 *           copy the pixel values axisymmetric about the boundaries.
 * Input: image data array pointer, image size.
 * Output: the pointer of the image array, the image size becomes width+4 * length+4.
 */
float* FilterBank(unsigned char* imagePtr, const int width,const int length,const int bytesPerPixel,const int windowSize){
    unsigned char originalImage[length][width][bytesPerPixel];
    unsigned char extendedImage[length+4][width+4][bytesPerPixel];
    //deal with boundary problem
    unsigned char *extendedImagePtr=ImageExtendBoundary(imagePtr, width, length, bytesPerPixel);

    //output image data, 25 images in total
    int numOfFilters=windowSize*windowSize;
    unsigned char filteredImage[numOfFilters][length][width][bytesPerPixel];
    float *featureVector=new float[numOfFilters];
    int i=0,j=0,dim=0;
    for(dim=0;dim<bytesPerPixel;dim++){
        for(i=0;i<length;i++){
            for(j=0;j<width;j++){
                originalImage[i][j][dim]=*(imagePtr+j+i*width+dim*width*length);//assign pixel values from the array pointer
            }
        }
        for(i=0;i<length+4;i++){
            for(j=0;j<width+4;j++){
                extendedImage[i][j][dim]=*(extendedImagePtr+j+i*width+dim*width*length);//assign pixel values from the array pointer
            }
        }
        
    }
    //store the filter coefficients of 25 filters
    char filterBanks[windowSize][windowSize][numOfFilters];
    //1d kernel filters bank[][] is in the order of L5,E5,S5,W5,R5
    const char bank[5][5]={{1,4,6,4,1},{-1,-2,0,2,1},{-1,0,2,0,-1},{-1,2,0,-2,1},{1,-4,6,-4,1}};
    /* 
     * Use tensor product to get filter masks
     *
     * Idea description:
     * i ranges from 0-24; for the first 5 filters, the tensor products are (L5)transpose ⊗ L5, E5
     * ...respectively, so programmed as bank[0]⊗bank[0],bank[1]...
     * For the next 5 filters, bank[1]⊗bank[0],bank[1]...,from observation use bank[int(i/windowSize)]
     * [m] as the filter reference.
     * The second multiplier changes every recursion of i,for example, the second result filter i=1,
     * is bank[0]⊗bank[1]; then for the 1st column of filter i=1, the values are the bank[0] transpose
     * multiplying the first value n=0 of bank[1][0].
     */
    int n=0;
    for(n=0;n<numOfFilters;n++){
        for(i=0;i<windowSize;i++){
            for(j=0;j<windowSize;j++){
                filterBanks[i][j][n]=bank[int(n/windowSize)][j]*bank[n%windowSize][i];
                //cout<<int(filterBanks[n][m][i])<<endl;
            }
        }
    }
    float avgImageEnergy[numOfFilters][dim];
    for(n=0;n<numOfFilters;n++){
        for(dim=0;dim<bytesPerPixel;dim++){
            avgImageEnergy[n][dim]=0;
            for(i=0;i<length;i++){
                for(j=0;j<width;j++){
                    filteredImage[n][i][j][dim]=0;
                    //using 5x5 window
                    for(int wl=0;wl<windowSize;wl++){
                        for(int ww=0;ww<windowSize;ww++){
                        //calculate masked center pixel value in the window
                            filteredImage[n][i][j][dim]+=filterBanks[wl][ww][n]*extendedImage[i+wl][j+ww][dim];
                            //filteredImage[i][j][dim]=255;
                        }
                    }
                    //calculate the total energy of image[n]
                    avgImageEnergy[n][dim]+=float(filteredImage[n][i][j][dim])*float(filteredImage[n][i][j][dim])/(length*width);
                    //cout<<float(filteredImage[n][i][j][dim])<<endl;
                    //subtract mean
                    //subtractedImage[j+i*width+dim*width*length]=originalImage[i][j][dim];
                }
            }
            //cout<<avgImageEnergy[n][dim]<<endl;
        }
    }
    
    for(n=0;n<numOfFilters;n++){
        for(dim=0;dim<bytesPerPixel;dim++){
            //feature vector is calculated by combining each filtered image's average energy
            featureVector[n]=avgImageEnergy[n][dim];
            //cout<<featureVector[n]<<endl;
        }
    }
    //test image
    //FILE *file;
    //file=fopen("/Users/liuhengyue/Google Drive/EE569/hw2/569HW2/569HW2/straw01filtered.raw","wb");
    //fwrite(filteredImage, sizeof(unsigned char), width*length*bytesPerPixel, file);
    //fclose(file);
    return featureVector;
}
/*
 * NormalizeFeature()
 * Function: This function reads in feature vectors after filter banks filtering;
 *           then normalize each feature by x'=(x-x_min)/(x_max-x_min)
 * Input: feature vectors array;window size=5.
 * Output: change the vector values of the original vector space (since define the vectors as static).
 */
void NormalizeFeature(float *vectors,const int windowSize){

        //find the max and min value of each feature
        float max=INT_MIN,min=INT_MAX;
        for(int j=0;j<windowSize*windowSize;j++){
            if(vectors[j]>max){
                max=vectors[j];
            }
            if(vectors[j]<min){
                min=vectors[j];
            }
        }
        //normalization
        for(int j=0;j<windowSize*windowSize;j++){
            vectors[j]=(vectors[j]-min)/(max-min);
        }
}
/*
 * Mahalanobis()
 * Function: This function reads two feature vectors and calculates its mahalanobis distance
 * Input: two feature vectors array; variances of 25 features; window size=5.
 * Output: mahalanobis distance of sample vector to straw mean vector
 */
bool Mahalanobis(float *sample, float *strawMeanVector,float *grassMeanVector, const float *strawVars,const float *grassVars,const int windowSize){
    float strawDistance=0,grassDistance=0;
    for(int i=0;i<windowSize*windowSize;i++){
        strawDistance=(sample[i]-strawMeanVector[i])*(sample[i]-strawMeanVector[i])/strawVars[i];
        grassDistance=(sample[i]-grassMeanVector[i])*(sample[i]-grassMeanVector[i])/grassVars[i];

    }
    strawDistance=sqrt(strawDistance);
    grassDistance=sqrt(grassDistance);
    if(strawDistance<grassDistance){
        return false;
    }
    else{
        return true;
    }
}
/*
 * imshowNewVectors()
 * Function: This function reads new feature vectors in Mat form and 
 *           method used for feature reduction, then normalize the coordinates,
 *           and draw vectors as 2D points for 1D or 2D feature vector space on
 *           a 400x400 image plane via imshow().
 * Input: feature vectors matrix;method= LDA or PCA.
 * Output: None. But show the image of reduced vector plane.
 */
void ImshowNewVectors(cv::Mat features,string method){
    //define a 400x400 color image
    cv::Mat vectorSpace=cv::Mat::zeros(400,400,CV_32FC3);
    //define dynamic window name
    stringstream fileIndex;
    //dim can be 0 or 1, only 1d or 2d feature vectors can be drawn
    int dim=0;
    if(features.cols==2){
        dim=1;
        fileIndex<<2;
    }
    else{
        fileIndex<<1;
    }
    //cout<<"dim "<<dim<<" "<<features.cols<<endl;
    string winName=fileIndex.str()+"D vector space via "+method;
    for(int col=0;col<features.cols;col++){
        //find each column's min and max
        double minVal=0,maxVal=0;
        cv::minMaxLoc(features.col(col), &minVal, &maxVal,0,0);
        for(int row=0;row<features.rows;row++){
            //for straw feature vectors,draw red points
            if(row<features.rows/2){
            //normalize scales to [0,1] then map to [0,400] and draw points
                circle(vectorSpace, cvPoint2D32f((features.at<Vec2f>(row,col)[0]-minVal)/(maxVal-minVal)*400, (features.at<Vec2f>(row,col)[dim]-minVal)/(maxVal-minVal)*400), 2, Scalar(0,0,255),-1,8,8);
            }
            //for grass feature vectors,draw green points
            else{
                circle(vectorSpace, cvPoint2D32f((features.at<Vec2f>(row,col)[0]-minVal)/(maxVal-minVal)*400, (features.at<Vec2f>(row,col)[dim]-minVal)/(maxVal-minVal)*400), 2, Scalar(0,255,0),-1,8,8);
            }
        }
        
    }
    cv::putText(vectorSpace, "red: straw", cvPoint(0,30), 0,0.5, Scalar(0,0,255));
    cv::putText(vectorSpace, "green: grass", cvPoint(0,60), 0,0.5, Scalar(0,255,0));
    //create window
    cv::namedWindow(winName,WINDOW_AUTOSIZE);
    cv::imshow(winName, vectorSpace);
    cvWaitKey(0);
    //cv::destroyWindow(winName);
    vectorSpace.release();
    //One little bug.refresh buffer? cannot destroy window,so fill in black
    imshow(winName, cv::Mat::zeros(400, 400, CV_32F));
    cv::destroyAllWindows();
}
/*
 * ReducedFeatureClassifier()
 * Function: This function reads 25D feature vectors in Mat form and test data
 *           feature vectors Mat,then form new basis from trainning 25D vectors;
 *           next, apply PCA or LDA dimension reduction to the test vectors, taking
 *           trainning images as test case as well. Final step, calculate Mahalanobis
 *           distance of test data to mean vector of two classes.
 *           Note: intuitive labels are recorded as 0 for straw, 1 for grass.
 * Input: 25D feature vectors Mat(); 25D test vectors Mat();
 *        NUMOFIMAGES=36;NUMOFUNKNOWN=24;correctLabels is labels array; components
 *        is the number of dimension after reduction;method= "LDA" or "PCA";
 *        numOfClasses is the number of classes used, like 2 or 4; targetClass index
 *        is like 0 for straw, 1 for grass as classifier, other samples will be 
 *        defined as non-class classifiers. Define -1 for unknown class.
 * Output: new feature vectors. Can show the image of reduced vector plane.
 */
//define return structure for next function
struct result{
    cv::Mat newFeatures;
    cv::Mat newTestFeatures;
    int *prediction;
    int count;
};
result ReducedFeatureClassifier(cv::Mat featureVectors25D,cv::Mat testVectors25D,const int numOfImages,const int numOfTests,int* testLabels,const int components,string method,const int numOfClasses,int targetClass){
    //define output string for display information
    string texureClass;
    cv::Mat projected(numOfClasses*numOfImages,components,CV_32F);//define projected features for trainning data
    cv::Mat newFeatures;//this Mat has less precision compared with Mat projected
    cv::Mat projectedTest(numOfTests,components,CV_32F);//define projected features for test data
    cv::Mat newTestFeatures;

    if(method=="PCA"){
        //PCA method to reduce feature vector dimension from 25 to n-component for total 96 images
        cv::PCA pca(featureVectors25D,cv::Mat(),0,components);//reduce to n-component feature
        pca.project(featureVectors25D,projected);
        projected.convertTo(newFeatures, CV_32F);
        pca.project(testVectors25D,projectedTest);//project test data
        projectedTest.convertTo(newTestFeatures, CV_32F);
        /*
        for(int i=0;i<numOfClasses*numOfImages;i++){
            
                cout<<newFeatures.row(i)<<endl;
            
        }*/
        
    }
    else if(method=="LDA"){
        //define labels for trainning data (arranged by classes in order)
        cv::Mat dataSetLabels(numOfClasses*numOfImages,1,CV_32F);
        //assign labels
        for(int n=0;n<numOfClasses;n++){
            //2 classess labeled as 0,1
            //4 classess labeled as 0,1,2,3
            for(int i=n*numOfImages;i<(n+1)*numOfImages;i++){
                dataSetLabels.at<float>(i)=n;
                //dataSetLabels.at<float>(i+NUMOFIMAGES)=1;
            }
        }
        cv::LDA lda(featureVectors25D, dataSetLabels,components);
        //cout<<projected.rows<<" "<<projected.cols<<endl;
        projected=lda.project(featureVectors25D);
        projected.convertTo(newFeatures, CV_32F);
        //cv::Mat projectedTest(NUMOFUNKNOWN,1,CV_32F);
        projectedTest=lda.project(testVectors25D);//project test data

        projectedTest.convertTo(newTestFeatures, CV_32F);
        //cout<<projected.row(0)<<endl<<projected.row(1)<<endl;
        //cout<<lda.eigenvectors().row(0)<<endl<<lda.eigenvectors().row(1)<<endl;

    }
    else{
        cout<<"input error"<<endl;
        exit(0);
    }
    //for cout error rates
    if(targetClass==-1){
        targetClass=0;
        texureClass="unknown";
    }
    else if(targetClass==0){
        texureClass="straw";
    }
    else if(targetClass==1){
        texureClass="grass";
    }
    else if(targetClass==2){
        texureClass="sand";
    }
    else if(targetClass==3){
        texureClass="leather";
    }
    else{
        cout<<"error of input targetClass."<<endl;
        exit(1);
    }

    //cout<<newFeatures.rows<<" "<<newFeatures.cols<<endl;
    //calculate mean and variance for target class images
    float classMean1D[components],classVar1D[components];
    for(int n=0;n<components;n++){
        classMean1D[n]=0;
        for(int j=targetClass*numOfImages;j<(targetClass+1)*numOfImages;j++){
            classMean1D[n]+=newFeatures.at<float>(j,n)/numOfImages;
            //cout<<feature1D.at<float>(j,n)/NUMOFIMAGES<<endl;
        }
    }
    for(int n=0;n<components;n++){
        classVar1D[n]=0;
        //find target class locations in newFeatures
        for(int j=targetClass*numOfImages;j<(targetClass+1)*numOfImages;j++){
            classVar1D[n]+=(newFeatures.at<float>(j,n)-classMean1D[n])*(newFeatures.at<float>(j,n)-classMean1D[n])/numOfImages;
        }
    }
    //calculate mean and variance for non-class images
    float nonClassMean1D[components],nonClassVar1D[components];
    for(int n=0;n<components;n++){
        nonClassMean1D[n]=0;
        for(int j=0;j<numOfClasses*numOfImages;j++){
            //exclude target class to calculate
            if(j<targetClass*numOfImages or j>=(targetClass+1)*numOfImages){
                nonClassMean1D[n]+=newFeatures.at<float>(j,n)/((numOfClasses-1)*numOfImages);
            }
        }
    }
    for(int n=0;n<components;n++){
        nonClassVar1D[n]=0;
        for(int j=0;j<numOfClasses*numOfImages;j++){
            if(j<targetClass*numOfImages or j>=(targetClass+1)*numOfImages){
                nonClassVar1D[n]+=(newFeatures.at<float>(j,n)-nonClassMean1D[n])*(newFeatures.at<float>(j,n)-nonClassMean1D[n])/((numOfClasses-1)*numOfImages);
            }
        }
    }

    /* calculate distances and record error labeled image for test images */
    //define labels
    int *predictLabels=new int[numOfTests];
    int binaryLabels[numOfTests];
    int errorLabeled=0;
    float errorRate=0;
    int count=0;
    for(int j=0;j<numOfTests;j++){
        float disToClass=0,disToNonClass=0;
        for(int n=0;n<components;n++){
            disToClass+=(newTestFeatures.at<float>(j,n)-classMean1D[n])*(newTestFeatures.at<float>(j,n)-classMean1D[n])/classVar1D[n];
            disToNonClass+=(newTestFeatures.at<float>(j,n)-nonClassMean1D[n])*(newTestFeatures.at<float>(j,n)-nonClassMean1D[n])/nonClassVar1D[n];
        }
        disToClass=sqrt(disToClass);
        disToNonClass=sqrt(disToNonClass);
        //cout<<disToStraw<<" "<<disToGrass<<endl;
        bool ifright=disToClass>disToNonClass;//1 means non-straw(grass),0 means straw
        predictLabels[j]=-1;//default unknown
        if(targetClass==testLabels[j]){
            binaryLabels[j]=0;
        }
        else{
            binaryLabels[j]=-1;
        }
        //cout<<binaryLabels[j]<<endl;
        if(ifright==binaryLabels[j]){// 0-labeled correctly, 1-labeled wrong
            predictLabels[j]=targetClass*(!ifright);
            count++;
        }
        
        //check if labeled right
        //cout<<correctLabels[j]<<endl;
        if(ifright*(-1)!=binaryLabels[j]){// 0-labeled correctly, 1-labeled wrong
            errorLabeled++;
            }
        }
    //cout<<"----------------"<<endl;
    /*
    for(int i=0;i<numOfTests;i++){
        if(predictLabels[i]==correctLabels[i]){
            count++;//count right classification
            }
        }
     */
    errorRate=float(errorLabeled)/float(numOfTests);
    //set solution to 4 classes case
    /*
    if(numOfTests==36){
        cout<<components<<"-D "<<method<<" error rate for classifying "<<texureClass<<" from "<<numOfTests<<" testing images is: "<<setprecision(4)<<errorRate*100/4<<"%."<<endl;
    }
    else{
        cout<<components<<"-D "<<method<<" error rate for classifying "<<texureClass<<" from "<<numOfTests<<" testing images is: "<<setprecision(4)<<errorRate*100<<"%."<<endl;
    }
     */
    //projected.release();
    result results;
    results.newFeatures=newFeatures;
    results.newTestFeatures=newTestFeatures;
    results.prediction=predictLabels;
    results.count=count;
    //ImshowNewVectors(newFeatures,"PCA");
    return results;
    }


/*
 * RandomIndex()
 * Function: This function selects non-repeated random numbers from [1,totalNum].
 * Input: number of selection; total number select from.
 * Output: an array of random numbers
 */

int* RandomIndex(int totalNum){
    int *indexes=new int[totalNum];
    for(int i=0; i<totalNum; i++){
        //number range [1,totalNum]. Ex: 1 to 48.
        indexes[i]=i+1;
    }
    for(int i=totalNum-1; i>0; i--){
        //swap the values beteen location i and random location
        swap(indexes[i], indexes[rand()%i]);
    }

    //indexes contain selectionNum of random numbers
    return indexes;
}
/*
 * CreateLabels()
 * Function: This function generate labels for test data set.
 * Input: const int numOfClass, const int numOfTest.
 * Output: an array of labels for test data, 0 straw , 1 grass, etc.
 */
int* CreateLabels(const int numOfClass, const int numOfTest){
    //for 4 classifiers, 4*12 is used
    int *classLabels=new int[numOfClass*numOfTest];
    //assign labels
    for(int n=0;n<numOfClass;n++){
        //since its binary classifier, assign 0 to target class, 1 to non-class
        for(int i=n*numOfTest;i<(n+1)*numOfTest;i++){
                classLabels[i]=n;
        }
    }
    return classLabels;
}
 int main(int argc, const char * argv[]) {
 
     //define image information variables
     const int WIDTH=128;
     const int LENGTH=128;
     const int BYTESPERPIXEL=1;
     //define file paths
     string READPATH="/Users/liuhengyue/Google Drive/EE569/hw2/HW2_images/P1/part a/";
     string WRITEPATH="/Users/liuhengyue/Google Drive/EE569/hw2/HW2_images/P1/Part a/results/";
     string STRAW="straw_";
     string GRASS="grass_";
     string UNKNOWN="unknown_";
     string EXT=".raw";
     string MODIFIER="test";
     //define number of sample images. 36 images as default for straw and grass,24 unknown ones.
     const int NUMOFIMAGES=36;
     const int NUMOFUNKNOWN=24;
     //define filer window size
     const int WINDOWSIZE=5;
     //define feature vectors array; the vectors will be normalized
     static float strawVectors[NUMOFIMAGES][WINDOWSIZE*WINDOWSIZE];
     static float grassVectors[NUMOFIMAGES][WINDOWSIZE*WINDOWSIZE];
     
     /********************************
      ** trainning data calculation **
      ********************************/
     
     /* Read all the straw files and grass files */
     //read file one by one and extract feature vector one by one
     cout<<"Reading trainning images..."<<endl;
     for(int i=0;i<NUMOFIMAGES;i++){
         stringstream fileIndex;
             if(i<9){
                      fileIndex<<0<<(i+1);//file name is like straw_01.raw
             }
             else{
                      fileIndex<<(i+1);//file name is like straw_11.raw
             }
         //compelete path of image sources
         string readStrawFullPath=READPATH+STRAW+fileIndex.str()+EXT;//read straw image
         string readGrassFullPath=READPATH+GRASS+fileIndex.str()+EXT;//read grass image
         //string writeFullPath=WRITEPATH+STRAW+fileIndex.str()+MODIFIER+EXT;
         //read the raw image
         unsigned char *strawImg=ReadRaw(readStrawFullPath.c_str(),WIDTH,LENGTH,BYTESPERPIXEL);
         unsigned char *grassImg=ReadRaw(readGrassFullPath.c_str(),WIDTH,LENGTH,BYTESPERPIXEL);
         //pre-process the image
         unsigned char *preProcessedStrawImg=MeanSubtract(strawImg, WIDTH, LENGTH, BYTESPERPIXEL);
         unsigned char *preProcessedGrassImg=MeanSubtract(grassImg, WIDTH, LENGTH, BYTESPERPIXEL);
         //filter the image with 25 filter bank, get  one 25 dimension feature vector
         float* strawFeatureVector=FilterBank(preProcessedStrawImg, WIDTH, LENGTH, BYTESPERPIXEL, WINDOWSIZE);
         float* grassFeatureVector=FilterBank(preProcessedGrassImg, WIDTH, LENGTH, BYTESPERPIXEL, WINDOWSIZE);
         for(int j=0;j<WINDOWSIZE*WINDOWSIZE;j++){
             strawVectors[i][j]=strawFeatureVector[j];
             grassVectors[i][j]=grassFeatureVector[j];

         }
         //normalize feature vector of image i (1-36)
         NormalizeFeature(strawVectors[i], WINDOWSIZE);
         NormalizeFeature(grassVectors[i], WINDOWSIZE);
         delete strawImg;
         delete grassImg;
         delete preProcessedStrawImg;
         delete preProcessedGrassImg;
     }
     // end of getting feature vectors
     
     /* calculate mean and variances of straw features and grass features */
     //define the mean and variance of each feature
     float strawVariances[WINDOWSIZE*WINDOWSIZE];
     float strawMeanVector[WINDOWSIZE*WINDOWSIZE];
     //fix feature index first and traversal each image's vector
     for(int j=0;j<WINDOWSIZE*WINDOWSIZE;j++){
         strawMeanVector[j]=0;
         strawVariances[j]=0;
         for(int i=0;i<NUMOFIMAGES;i++){
             //find the mean vector of all straw feature vector
             strawMeanVector[j]+=strawVectors[i][j]/NUMOFIMAGES;
         }
         for(int i=0;i<NUMOFIMAGES;i++){
             //find the variances of each feature
             strawVariances[j]+=(strawVectors[i][j]-strawMeanVector[j])*(strawVectors[i][j]-strawMeanVector[j])/NUMOFIMAGES;
         }

     }
     
     //define the mean and variance of each feature
     float grassVariances[WINDOWSIZE*WINDOWSIZE];
     float grassMeanVector[WINDOWSIZE*WINDOWSIZE];
     //fix feature index first and traversal each image's vector
     for(int j=0;j<WINDOWSIZE*WINDOWSIZE;j++){
         grassMeanVector[j]=0;
         grassVariances[j]=0;
         for(int i=0;i<NUMOFIMAGES;i++){
             //find the mean vector of all straw feature vector
             grassMeanVector[j]+=grassVectors[i][j]/NUMOFIMAGES;
         }
         for(int i=0;i<NUMOFIMAGES;i++){
             //find the variances of each feature
             grassVariances[j]+=(grassVectors[i][j]-grassMeanVector[j])*(grassVectors[i][j]-grassMeanVector[j])/NUMOFIMAGES;
         }
         
     }
     cout<<"Done! Labeling via 25D feature..."<<endl;
     /* end of mean and variances calculation */
     
     /***********************************************
      **         test data calculation             **
      ***********************************************/
     
     //define the feature vectors for 24 unknown images
     static float testVectors[NUMOFUNKNOWN][WINDOWSIZE*WINDOWSIZE];
     //define the intuitive classification of 24 unknown texture images,0(false) stands for straw,
     //and 1(true) for grass.
     int correctLabels[NUMOFUNKNOWN]={0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,0,0,1,1,1,0,0,1};
     int errorLabeled=0;
     float errorRate=0;
     for(int i=0;i<NUMOFUNKNOWN;i++){
         stringstream fileIndex;
         if(i<9){
             fileIndex<<0<<(i+1);//file name is like straw_01.raw
         }
         else{
             fileIndex<<(i+1);//file name is like straw_11.raw
         }
         string testPath=READPATH+UNKNOWN+fileIndex.str()+EXT;
         //read the raw image
         unsigned char *testImg=ReadRaw(testPath.c_str(),WIDTH,LENGTH,BYTESPERPIXEL);
         //pre-process the image
         unsigned char *testpreProcessedImg=MeanSubtract(testImg, WIDTH, LENGTH, BYTESPERPIXEL);
         //filter the image with 25 filter bank, get  one 25 dimension feature vector
         float* testVector=FilterBank(testpreProcessedImg, WIDTH, LENGTH, BYTESPERPIXEL, WINDOWSIZE);

         for(int j=0;j<WINDOWSIZE*WINDOWSIZE;j++){
             testVectors[i][j]=testVector[j];
         }
         //normalize feature vector of image i (1-24)
         NormalizeFeature(testVectors[i], WINDOWSIZE);
         //classify using Mahalanobis distance and count error rate
         if(correctLabels[i]!=Mahalanobis(testVectors[i], strawMeanVector,grassMeanVector,strawVariances,grassVariances,WINDOWSIZE)){
             errorLabeled+=1;//record error labeled image number
            }
         
         delete testImg;
         delete testpreProcessedImg;
     }
     errorRate=float(errorLabeled)/float(NUMOFUNKNOWN);//calculate error rate
     cout<<"Done!"<<endl<<"Using 25D feature vector, the error rate is: "<<errorRate*100<<"%."<<endl;
     //Problem 1 (a) 2) results
     //end of 25-D feature vector classifier
     
     /*************************************************
      **  From here, using opencv for P1 (a) 2)-6)   **
      ************************************************/
     
     //define cv::Mat() for trainning and testing data
     cv::Mat featureVectors25D(NUMOFIMAGES*2,WINDOWSIZE*WINDOWSIZE,CV_32F);
     cv::Mat testVectors25D(NUMOFUNKNOWN,WINDOWSIZE*WINDOWSIZE,CV_32F);
     /* Convert float[] array to cv:Mat() matrix */
     //trainning data
     for(int col=0;col<WINDOWSIZE*WINDOWSIZE;col++){
         int row=0;
         for(row=0;row<NUMOFIMAGES;row++){
             featureVectors25D.at<float>(row,col)=strawVectors[row][col];
            }
         for(row=NUMOFIMAGES;row<2*NUMOFIMAGES;row++){
             featureVectors25D.at<float>(row,col)=grassVectors[row-NUMOFIMAGES][col];
            }
     }
     //testing data
     for(int col=0;col<WINDOWSIZE*WINDOWSIZE;col++){
         for(int row=0;row<NUMOFUNKNOWN;row++){
             testVectors25D.at<float>(row,col)=testVectors[row][col];
         }
     }
     //end of forming Mat()
     
     /************
      **  PCA   **
      ************/
     cout<<"PCA..."<<endl;
     //define right label count
     int totalCount=0;
     //define target labels
     int testlabels[2][NUMOFIMAGES];
     for(int i=0;i<2;i++){
         for(int j=0;j<NUMOFIMAGES;j++){
             testlabels[i][j]=i;
         }
     }
     //int *testlabels=CreateLabels(2, NUMOFIMAGES, 0);
     result result1=ReducedFeatureClassifier(featureVectors25D, testVectors25D, NUMOFIMAGES, NUMOFUNKNOWN, correctLabels,1,"PCA",2,0);
     //cout<<result1.count<<endl;
     result result2=ReducedFeatureClassifier(featureVectors25D, testVectors25D, NUMOFIMAGES, NUMOFUNKNOWN, correctLabels,1,"PCA",2,1);
     
     /****** use this function for display new feature space *****/
     //ImshowNewVectors(result1.newFeatures, "PCA");
     totalCount=result1.count+result2.count;
     cout<<"1-D PCA error rate for classifying from 24 testing images is: "<<100*float(NUMOFUNKNOWN-totalCount)/NUMOFUNKNOWN<<"%."<<endl<<"**************** "<<endl;
     result result3=ReducedFeatureClassifier(featureVectors25D, featureVectors25D.rowRange(0, NUMOFIMAGES), NUMOFIMAGES, NUMOFIMAGES, testlabels[0],1,"PCA",2,0);
         //cout<<result3.count<<endl;
     //f/or(int i=0;i<NUMOFIMAGES;i++){
         //cout<<result3.prediction[i]<<" "<<testlabels[0][i]<<endl;
     //}
     //testlabels=CreateLabels(2, NUMOFIMAGES, 1);
     result result4=ReducedFeatureClassifier(featureVectors25D, featureVectors25D.rowRange(NUMOFIMAGES, 2*NUMOFIMAGES), NUMOFIMAGES, NUMOFIMAGES, testlabels[1],1,"PCA",2,1);
         //cout<<result4.count<<endl;
     totalCount=result3.count+result4.count;
     cout<<"1-D PCA error rate for classifying from 72 training images is: "<<float(2*NUMOFIMAGES-totalCount)/(2*NUMOFIMAGES)<<"%."<<endl<<"**************** "<<endl;
     
     //newFeatures.release();
     /************
      **  LDA   **
      ************/
     cout<<"LDA..."<<endl;
     //only with 2 classes, the dimension can only be reduced to 1
     totalCount=0;
     for(int i=0;i<2;i++){
         result ldaResult=ReducedFeatureClassifier(featureVectors25D, testVectors25D, NUMOFIMAGES, NUMOFUNKNOWN, correctLabels,1,"LDA",2,i);
         //cout<<ldaResult.count<<endl;
         totalCount+=ldaResult.count;
     
     }
     cout<<"1-D LDA error rate for classifying from 24 testing images is: "<<100*float(NUMOFUNKNOWN-totalCount)/NUMOFUNKNOWN<<"%."<<endl<<"**************** "<<endl;

     totalCount=0;
     for(int i=0;i<2;i++){
         result ldaResult=ReducedFeatureClassifier(featureVectors25D,  featureVectors25D.rowRange(i*NUMOFIMAGES, (i+1)*NUMOFIMAGES), NUMOFIMAGES, NUMOFIMAGES, testlabels[i],1,"LDA",2,i);
         //cout<<ldaResult.count<<endl;
         totalCount+=ldaResult.count;
         
     }
     cout<<"1-D LDA error rate for classifying from 72 training images is: "<<100*float(2*NUMOFIMAGES-totalCount)/(2*NUMOFIMAGES)<<"%."<<endl<<"**************** "<<endl;
     //end of problem 1 (a)
     
     /**************************************************************
      ****   P1 (b)              4 classifiers                  ****
      **************************************************************/
     cout<<"4 classes classifier..."<<endl;
     //define variables for problem 1 (b)
     const int CLASS=4;//4 classifiers
     const int TESTNUM=12;//randomly select 12 from 48 as test data
     const int SAMPLENUM=48;//48 number of images for each class
     const int TRAINNUM=SAMPLENUM-TESTNUM;//36 trainning images for each class
     const string SAND="sand_";
     const string LEATHER="leather_";
     READPATH="/Users/liuhengyue/Google Drive/EE569/hw2/HW2_images/P1/part b/";
     int *indexes=RandomIndex(SAMPLENUM);//randomly arrange 1-48 indexes
     //define feature vectors array; the vectors will be normalized
     static float fourClassesVectors[CLASS*SAMPLENUM][WINDOWSIZE*WINDOWSIZE];
     bool labels[CLASS*TESTNUM];
     for(int i=0;i<TESTNUM;i++){
         labels[i]=0;//straw
         labels[i+TESTNUM]=1;//grass
         labels[i+2*TESTNUM]=2;//sand
         labels[i+3*TESTNUM]=3;//leather
     }
     /* Read all 4 classes' files */
     //read file one by one and extract feature vector one by one
     cout<<"Reading all images..."<<endl;
     for(int i=0;i<SAMPLENUM;i++){
         stringstream fileIndex;
         if(indexes[i]<10){
             fileIndex<<0<<indexes[i];//file name is like straw_01.raw
         }
         else{
             fileIndex<<indexes[i];//file name is like straw_11.raw
         }
         //compelete path of image sources
         string readStrawFullPath=READPATH+STRAW+fileIndex.str()+EXT;//read straw image
         string readGrassFullPath=READPATH+GRASS+fileIndex.str()+EXT;//read grass image
         string readSandFullPath=READPATH+SAND+fileIndex.str()+EXT;//read straw image
         string readLeatherFullPath=READPATH+LEATHER+fileIndex.str()+EXT;//read grass image
         //get image data
         unsigned char *strawImg=ReadRaw(readStrawFullPath.c_str(),WIDTH,LENGTH,BYTESPERPIXEL);
         unsigned char *grassImg=ReadRaw(readGrassFullPath.c_str(),WIDTH,LENGTH,BYTESPERPIXEL);
         unsigned char *sandImg=ReadRaw(readSandFullPath.c_str(),WIDTH,LENGTH,BYTESPERPIXEL);
         unsigned char *leatherImg=ReadRaw(readLeatherFullPath.c_str(),WIDTH,LENGTH,BYTESPERPIXEL);
         //pre-process the image
         unsigned char *preProcessedStrawImg=MeanSubtract(strawImg, WIDTH, LENGTH, BYTESPERPIXEL);
         unsigned char *preProcessedGrassImg=MeanSubtract(grassImg, WIDTH, LENGTH, BYTESPERPIXEL);
         unsigned char *preProcessedSandImg=MeanSubtract(sandImg, WIDTH, LENGTH, BYTESPERPIXEL);
         unsigned char *preProcessedLeatherImg=MeanSubtract(leatherImg, WIDTH, LENGTH, BYTESPERPIXEL);
         //filter the image with 25 filter bank, get  one 25 dimension feature vector
         float* strawFeatureVector=FilterBank(preProcessedStrawImg, WIDTH, LENGTH, BYTESPERPIXEL, WINDOWSIZE);
         float* grassFeatureVector=FilterBank(preProcessedGrassImg, WIDTH, LENGTH, BYTESPERPIXEL, WINDOWSIZE);
         float* sandFeatureVector=FilterBank(preProcessedSandImg, WIDTH, LENGTH, BYTESPERPIXEL, WINDOWSIZE);
         float* leatherFeatureVector=FilterBank(preProcessedLeatherImg, WIDTH, LENGTH, BYTESPERPIXEL, WINDOWSIZE);
         //construct all feature vectors
         for(int j=0;j<WINDOWSIZE*WINDOWSIZE;j++){
             fourClassesVectors[i][j]=strawFeatureVector[j];
             fourClassesVectors[i+SAMPLENUM][j]=grassFeatureVector[j];
             fourClassesVectors[i+2*SAMPLENUM][j]=sandFeatureVector[j];
             fourClassesVectors[i+3*SAMPLENUM][j]=leatherFeatureVector[j];
             
         }
         //normalize feature vectors
         NormalizeFeature(fourClassesVectors[i], WINDOWSIZE);
         NormalizeFeature(fourClassesVectors[i+SAMPLENUM], WINDOWSIZE);
         NormalizeFeature(fourClassesVectors[i+2*SAMPLENUM], WINDOWSIZE);
         NormalizeFeature(fourClassesVectors[i+3*SAMPLENUM], WINDOWSIZE);
         //delete char variables
         delete strawImg;
         delete grassImg;
         delete sandImg;
         delete leatherImg;

         delete preProcessedStrawImg;
         delete preProcessedGrassImg;
         delete preProcessedSandImg;
         delete preProcessedLeatherImg;

     }
     // end of getting feature vectors
     /* Convert float[] array to cv:Mat() matrix */
     //re-create Mat() using pre-defined variable; SAMPLENUM-TESTNUM=36
     featureVectors25D.create(4*TRAINNUM, WINDOWSIZE*WINDOWSIZE, CV_32F);
     testVectors25D.create(4*TESTNUM, WINDOWSIZE*WINDOWSIZE, CV_32F);
     //trainning data 36*4 x 25
     for(int col=0;col<WINDOWSIZE*WINDOWSIZE;col++){
         int row=0;
         for(row=0;row<TRAINNUM;row++){
             featureVectors25D.at<float>(row,col)=fourClassesVectors[row][col];
         }
         for(row=TRAINNUM;row<2*TRAINNUM;row++){
             featureVectors25D.at<float>(row,col)=fourClassesVectors[row+TESTNUM][col];
         }
         for(row=2*TRAINNUM;row<3*TRAINNUM;row++){
             featureVectors25D.at<float>(row,col)=fourClassesVectors[row+2*TESTNUM][col];
         }
         for(row=3*TRAINNUM;row<4*TRAINNUM;row++){
             featureVectors25D.at<float>(row,col)=fourClassesVectors[row+3*TESTNUM][col];
         }
     }
     //testing data 12*4 x 25
     for(int col=0;col<WINDOWSIZE*WINDOWSIZE;col++){
         int row=0;
         for(row=0;row<TESTNUM;row++){
             testVectors25D.at<float>(row,col)=fourClassesVectors[row+TRAINNUM][col];
         }
         for(row=TESTNUM;row<2*TESTNUM;row++){
             testVectors25D.at<float>(row,col)=fourClassesVectors[row+2*TRAINNUM][col];
         }
         for(row=2*TESTNUM;row<3*TESTNUM;row++){
             testVectors25D.at<float>(row,col)=fourClassesVectors[row+3*TRAINNUM][col];
         }
         for(row=3*TESTNUM;row<4*TESTNUM;row++){
             testVectors25D.at<float>(row,col)=fourClassesVectors[row+4*TRAINNUM][col];
         }
     }

     //int predictedLabels[CLASS][CLASS*TESTNUM];
     //int totalLabels[CLASS];
     //create 4 target class labels
     int *test48=CreateLabels(CLASS, TESTNUM);
     int *test144=CreateLabels(CLASS, TRAINNUM);
     //for(int i=0;i<CLASS*TESTNUM;i++){
       //  cout<<targetLabels[i]<<endl;
     //}
     /*  PCA  */
     result pcaResult;
     cout<<"PCA..."<<endl;
     //48 testing data
     cout<<"**************** "<<endl<<"For 48 test images, PCA results:"<<endl;
     totalCount=0;
     for(int i=0;i<CLASS;i++){
         pcaResult=ReducedFeatureClassifier(featureVectors25D, testVectors25D, TRAINNUM, CLASS*TESTNUM, test48,3,"PCA",CLASS,i);
         totalCount+=pcaResult.count;
     }
     cout<<"3-D PCA error rate for classifying from 48 testing images is: "<<100*float(TESTNUM-float(totalCount/CLASS))/TESTNUM<<"%."<<endl<<"**************** "<<endl;
     //4 classes data from trainning data
     cout<<"**************** "<<endl<<"For 144 traing images as test data, PCA results:"<<endl;
     totalCount=0;
     for(int i=0;i<CLASS;i++){
     pcaResult=ReducedFeatureClassifier(featureVectors25D, featureVectors25D, TRAINNUM, TRAINNUM, test144,3,"PCA",CLASS,i);
         totalCount+=pcaResult.count;
     }
     cout<<"3-D PCA error rate for classifying from 144 training images is: "<<100*float(TRAINNUM-float(totalCount/CLASS))/TRAINNUM<<"%."<<endl<<"**************** "<<endl;
     /*  LDA  */
     result ldaResult;
     //48 testing data
     cout<<"**************** "<<endl<<"For 48 test images, LDA results:"<<endl;
     totalCount=0;
     for(int i=0;i<CLASS;i++){
         ldaResult=ReducedFeatureClassifier(featureVectors25D, testVectors25D, TRAINNUM, CLASS*TESTNUM, test48,3,"LDA",CLASS,i);
         totalCount+=ldaResult.count;
     }
     cout<<"3-D LDA error rate for classifying from 48 testing images is: "<<100*float(TESTNUM-float(totalCount/CLASS))/TESTNUM<<"%."<<endl<<"**************** "<<endl;
     
     //4 classes data from trainning data
     cout<<"**************** "<<endl<<"For 144 traing images as test data, PCA results:"<<endl;
     //cout<<"For each 36 group of training class images, LDA results:"<<endl;
     totalCount=0;
     for(int i=0;i<CLASS;i++){
         ldaResult=ReducedFeatureClassifier(featureVectors25D, featureVectors25D, TRAINNUM, TRAINNUM, test144,3,"LDA",CLASS,i);
         totalCount+=ldaResult.count;
     }
     cout<<"3-D LDA error rate for classifying from 144 training images is: "<<100*float(TRAINNUM-float(totalCount/CLASS))/TRAINNUM<<"%."<<endl<<"**************** "<<endl;
     //end of Problem 1 b) 2)
     
     /****************************
      ****       SVM          ****
      ****************************/
      // Set up SVM's parameters

     CvSVMParams params;
     params.svm_type    = CvSVM::C_SVC;
     params.kernel_type = CvSVM::LINEAR;
     params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
     //define labels
     Mat trainLabelsMat(CLASS*TRAINNUM, 1, CV_32FC1);
     Mat testLabelsMat(CLASS*TESTNUM, 1, CV_32FC1);
     for(int c=0;c<CLASS;c++){
         for(int row=c*TRAINNUM;row<(c+1)*TRAINNUM;row++){
             trainLabelsMat.at<float>(row,0)=c;
        }
         for(int row=c*TESTNUM;row<(c+1)*TESTNUM;row++){
             testLabelsMat.at<float>(row,0)=c;
             //cout<<testLabelsMat.at<float>(row,0)<<endl;
         }

     }
     //define error number
     int errorNum=0;
     // Train the SVM
     CvSVM SVM;
     
     //using PCA
     pcaResult=ReducedFeatureClassifier(featureVectors25D, testVectors25D, TRAINNUM, CLASS*TESTNUM, test48,3,"PCA",CLASS,0);
     SVM.train(pcaResult.newFeatures, trainLabelsMat, Mat(), Mat(), params);
     for(int i=0;i<pcaResult.newTestFeatures.rows;i++){
         float response= SVM.predict(pcaResult.newTestFeatures.row(i));
         //cout<<"SVM Response is: "<<response<<" "<<testLabelsMat.at<float>(i,0)<<endl;
         if(response!=testLabelsMat.at<float>(i,0)){
             errorNum++;
         }
     }
     cout<<"SVM error rate via PCA 3D feature vectors is: "<<setprecision(4)<<float(errorNum)/pcaResult.newTestFeatures.rows*100<<"%"<<endl;
     //test training data
     errorNum=0;
     for(int i=0;i<pcaResult.newFeatures.rows;i++){
         float response= SVM.predict(pcaResult.newFeatures.row(i));
         //cout<<"SVM Response is: "<<response<<" "<<trainLabelsMat.at<float>(i,0)<<endl;
         if(response!=trainLabelsMat.at<float>(i,0)){
             errorNum++;
         }
     }
     cout<<"SVM error rate for 144 training images via PCA 3D feature vectors is: "<<setprecision(4)<<float(errorNum)/pcaResult.newFeatures.rows*100<<"%"<<endl;
      
     //train using LDA
     result ldaResults=ReducedFeatureClassifier(featureVectors25D, testVectors25D, TRAINNUM, CLASS*TESTNUM, test48,3,"LDA",CLASS,0);
     SVM.train(ldaResults.newFeatures, trainLabelsMat, Mat(), Mat(), params);
     for(int i=0;i<ldaResults.newTestFeatures.rows;i++){
         float response= SVM.predict(ldaResults.newTestFeatures.row(i));
         //cout<<"SVM Response is: "<<response<<" "<<testLabelsMat.at<float>(i,0)<<endl;
         if(response!=testLabelsMat.at<float>(i,0)){
             errorNum++;
         }
     }
     cout<<"SVM error rate via LDA 3D feature vectors is: "<<setprecision(4)<<float(errorNum)/ldaResult.newTestFeatures.rows*100<<"%"<<endl;
     //test training data
     errorNum=0;
     for(int i=0;i<ldaResult.newFeatures.rows;i++){
         float response= SVM.predict(ldaResult.newFeatures.row(i));
         //cout<<"SVM Response is: "<<response<<" "<<trainLabelsMat.at<float>(i,0)<<endl;
         if(response!=trainLabelsMat.at<float>(i,0)){
             errorNum++;
         }
     }
     cout<<"SVM error rate for 144 training images via LDA 3D feature vectors is: "<<setprecision(4)<<float(errorNum)/pcaResult.newFeatures.rows*100<<"%"<<endl;
    return 0;
}


//problem 2

//
//  main.cpp
//  HW2P2
//
//  Created by LiuHengyue on 10/7/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <math.h>
#include<iomanip>
#include <algorithm>
#include <limits>
//opencv header files
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <iostream>
//raw file methods
#include "raw.h"
using namespace std;
using namespace cv;
/*
 * imshowNewVectors()
 * Function: This function reads new feature vectors in Mat form and
 *           method used for feature reduction, then normalize the coordinates,
 *           and draw vectors as 2D points for 1D or 2D feature vector space on
 *           a 400x400 image plane via imshow().
 * Input: feature vectors matrix;method= LDA or PCA.
 * Output: None. But show the image of reduced vector plane.
 */
bool complare(int a,int b) {
    return a>b;
}//for sort alogorithm descending order
Mat ReduceEdge(Mat gradMat,float percent){
    //count the total number of original gradient map
    int count=0, newCount=0;
    //get the number of edge points reduced to, like 10% points are edge points
    int numOfEdgePoints=gradMat.size[0]*gradMat.size[1]*percent;
    int gradArray[gradMat.size[0]*gradMat.size[1]];
    //copy the mat
    Mat gradCopy;
    gradMat.copyTo(gradCopy);
    for(int i=0;i<gradCopy.rows;i++){
        for(int j=0;j<gradCopy.cols;j++){
            gradArray[j+i*gradCopy.cols]=gradCopy.at<unsigned char>(i,j);
            //cout<<gradArray[j+i*gradMat.rows]<<endl;
            if(gradCopy.at<unsigned char>(i,j)>0){
                count++;
            }
        }
    }
    //sort the array from the largest to smallest
    std::sort(gradArray, gradArray+gradCopy.size[0]*gradCopy.size[1],complare);
    //set threshold pixel value
    int threshold=gradArray[numOfEdgePoints-1];
    //cout<<numOfEdgePoints<<" "<<" "<<threshold<<endl;
    //reduce edge points
    newCount=gradCopy.size[0]*gradCopy.size[1];
        for(int i=0;i<gradCopy.rows;i++){
            for(int j=0;j<gradCopy.cols;j++){
                if(gradCopy.at<unsigned char>(i,j)<=threshold){
                    gradCopy.at<unsigned char>(i,j)=0;
                    newCount--;
                    }
                }
        }

    cout<<"Original gradient map has "<<100*float(count)/(gradCopy.size[0]*gradCopy.size[1])<<"% of points are edge points."<<endl;
    cout<<"New reduced gradient map has "<<100*float(newCount)/(gradCopy.size[0]*gradCopy.size[1])<<"% of points are edge points."<<endl;

    return gradCopy;
}
/*
 * nonMaximaSuppression produces a mask (dst) such that every non-zero
 * value of the mask corresponds to a local maxima of src.
 * Input: source Mat();sz the size of the window;window mask.
 * Output: suppressed gradient image.
 */
void nonMaximaSuppression(const Mat& src, const int windowSize, Mat& dst, const Mat mask) {
    
    // initialise the block mask and destination
    const int M = src.rows;
    const int N = src.cols;
    const bool masked = !mask.empty();
    Mat block = 255*Mat_<uint8_t>::ones(Size(2*windowSize+1,2*windowSize+1));
    dst = Mat_<uint8_t>::zeros(src.size());
    
    // iterate over image blocks  step can be sz+1 if shift by block
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            Point  ijmax;
            double vcmax, vnmax;
            
            // get the maximal candidate within the block
            Range ic(m, min(m+windowSize+1,M));
            Range jc(n, min(n+windowSize+1,N));
            minMaxLoc(src(ic,jc), NULL, &vcmax, NULL, &ijmax, masked ? mask(ic,jc) : noArray());
            Point cc = ijmax + Point(jc.start,ic.start);
            
            // search the neighbours centered around the candidate for the true maxima
            Range in(max(cc.y-windowSize,0), min(cc.y+windowSize+1,M));
            Range jn(max(cc.x-windowSize,0), min(cc.x+windowSize+1,N));
            
            // mask out the block whose maxima we already know
            
            Mat_<uint8_t> blockmask;
            block(Range(0,in.size()), Range(0,jn.size())).copyTo(blockmask);
            Range iis(ic.start-in.start, min(ic.start-in.start+windowSize+1, in.size()));
            Range jis(jc.start-jn.start, min(jc.start-jn.start+windowSize+1, jn.size()));
            blockmask(iis, jis) = Mat_<uint8_t>::zeros(Size(jis.size(),iis.size()));
            
            minMaxLoc(src(in,jn), NULL, &vnmax, NULL, &ijmax, masked ? mask(in,jn).mul(blockmask) : blockmask);
            //Point cn = ijmax + Point(jn.start, in.start);
            
            // if the block centre is also the neighbour centre, then it's a local maxima
            if (vcmax > vnmax) {
                dst.at<uint8_t>(cc.y, cc.x) = vcmax;//change value to local maxima
            }
        }
    }
}
Mat readMat2Grey(const int length,const int width,const int bytesPerPixel,const char* path){
    //read raw image into array
    unsigned char* imgPtr=ReadRaw(path, width, length, bytesPerPixel);
    //define matrice
    cv::Mat imgMat(length,width,CV_8UC3);
    cv::Mat imgGrey(length,width,CV_8UC1);
    int i=0,j=0;
    //WriteRaw("/Users/liuhengyue/Google Drive/EE569/hw2/Farm_x.raw", FARM, length, width, bytesPerPixel);
    //since opencv uses BGR, change the channel order of image array
    
    for(i=0;i<length;i++){//array row
        for(j=0;j<width;j++){//coloumn
            //since opencv uses BGR, change the channel order of image array
            //BGR->RGB
            imgMat.at<Vec3b>(i,j)[0]=imgPtr[(i*width+j)*bytesPerPixel+bytesPerPixel-1];
            imgMat.at<Vec3b>(i,j)[1]=imgPtr[(i*width+j)*bytesPerPixel+bytesPerPixel-2];
            imgMat.at<Vec3b>(i,j)[2]=imgPtr[(i*width+j)*bytesPerPixel+bytesPerPixel-3];
            imgGrey.at<unsigned char>(i,j)=0.21*imgMat.at<Vec3b>(i,j)[0]+0.72*imgMat.at<Vec3b>(i,j)[1]+0.07*imgMat.at<Vec3b>(i,j)[2];//Y=0.21R+0.72G+0.07B
            //cout<<farmGrad.at<unsigned char>(i,j)<<endl;
        }
    }
    return imgGrey;
}
Mat GradientMap(Mat src){
    //define matrice
    cv::Mat imgGrad(src.rows,src.cols,CV_16S);//avoid overflow
    cv::Mat imgGradAbs(src.rows,src.cols,CV_8UC1);
    //call Sobel filter
    //Or use filterbank function in Problem 1, it is easy to design the Sobel Mask
    cv::Sobel(src, imgGrad, CV_8UC1, 1, 1,3,1,0,BORDER_DEFAULT);
    //change values to 0-255
    convertScaleAbs(imgGrad, imgGradAbs);
    return imgGradAbs;
}
int main(int argc, const char * argv[]) {
    //define basic image info
    int LENGTH=321;
    int WIDTH=481;
    int BYTES_PER_PIXEL=3;
    const char* FARM_PATH="/Users/liuhengyue/Google Drive/EE569/hw2/HW2_images/P2/Farm.raw";
    const char* COUGAR_PATH="/Users/liuhengyue/Google Drive/EE569/hw2/HW2_images/P2/Cougar.raw";
    Mat farmGrey=readMat2Grey(LENGTH,WIDTH,BYTES_PER_PIXEL,FARM_PATH);
    Mat cougarGrey=readMat2Grey(LENGTH,WIDTH,BYTES_PER_PIXEL,COUGAR_PATH);
    Mat farmEdge,cougarEdge;
    //get gradient maps
    /*
    //Farm image
    cout<<"Farm image results:"<<endl;
    Mat farmGradAbs=GradientMap(farmGrey);
    imshow("original farm edge gradient map", farmGradAbs);
    waitKey(0);
    cv:Mat farm_10Pct=ReduceEdge(farmGradAbs, 0.1);
    imshow("10% edge gradient map",farm_10Pct);
    waitKey(0);
    Mat farm_15Pct=ReduceEdge(farmGradAbs, 0.15);
    imshow("15% edge gradient map",farm_15Pct);
    waitKey(0);
    //non-maximum suppression
    Mat farm_10PctNMS,farm_15PctNMS;
    Mat mask_10Pct=(farm_10Pct > 0);
    nonMaximaSuppression(farm_10Pct, 1, farm_10PctNMS, mask_10Pct);
    imshow("10% edge gradient map with NMS", farm_10PctNMS);
    waitKey(0);
    Mat mask_15Pct=(farm_15Pct > 0);
    nonMaximaSuppression(farm_15Pct, 1, farm_15PctNMS, mask_15Pct);
    imshow("15% edge gradient map with NMS", farm_15PctNMS);
    waitKey(0);
     */
    //Canny edge detector
    Canny(farmGrey, farmEdge, 0.3*255, 0.6*255);
    imshow("Canny Farm edges with A: 0.3, B 0.6", farmEdge);
    waitKey(0);
    Canny(farmGrey, farmEdge, 0.2*255, 0.7*255);
    imshow("Canny Farm edges with A: 0.2, B 0.7", farmEdge);
    waitKey(0);
    Canny(farmGrey, farmEdge, 0.2*255, 0.5*255);
    imshow("Canny Farm edges with A: 0.2, B 0.5", farmEdge);
    waitKey(0);
    Canny(farmGrey, farmEdge, 0.4*255, 0.7*255);
    imshow("Canny Farm edges with A: 0.4, B 0.7", farmEdge);
    waitKey(0);
    Canny(farmGrey, farmEdge, 0.4*255, 0.5*255);
    imshow("Canny Farm edges with A: 0.4, B 0.5", farmEdge);
    waitKey(0);


    /*
    //Cougar image
    cout<<"Cougar image results:"<<endl;
    Mat cougarGradAbs=GradientMap(LENGTH,WIDTH,BYTES_PER_PIXEL,COUGAR_PATH);
    imshow("original cougar edge gradient map", cougarGradAbs);
    waitKey(0);
    Mat cougar_10Pct=ReduceEdge(cougarGradAbs, 0.1);
    imshow("10% edge gradient map",cougar_10Pct);
    waitKey(0);
    Mat cougar_15Pct=ReduceEdge(cougarGradAbs, 0.15);
    imshow("15% edge gradient map",cougar_15Pct);
    waitKey(0);
    //non-maximum suppression
    Mat cougar_10PctNMS,cougar_15PctNMS;
    Mat mask_10Pct=(cougar_10Pct > 0);
    nonMaximaSuppression(cougar_10Pct, 1, cougar_10PctNMS, mask_10Pct);
    imshow("10% edge gradient map with NMS", cougar_10PctNMS);
    waitKey(0);
    Mat mask_15Pct=(cougar_15Pct > 0);
    nonMaximaSuppression(cougar_15Pct, 1, cougar_15PctNMS, mask_15Pct);
    imshow("15% edge gradient map with NMS", cougar_15PctNMS);
    waitKey(0);
     */
    //Canny edge detector
    Canny(cougarGrey, cougarEdge, 0.3*255, 0.6*255);
    imshow("Canny cougar edges with A: 0.3, B 0.6", cougarEdge);
    waitKey(0);
    Canny(cougarGrey, cougarEdge, 0.2*255, 0.7*255);
    imshow("Canny cougar edges with A: 0.2, B 0.7", cougarEdge);
    waitKey(0);
    Canny(cougarGrey, cougarEdge, 0.2*255, 0.5*255);
    imshow("Canny cougar edges with A: 0.2, B 0.5", cougarEdge);
    waitKey(0);
    Canny(cougarGrey, cougarEdge, 0.4*255, 0.7*255);
    imshow("Canny cougar edges with A: 0.4, B 0.7", cougarEdge);
    waitKey(0);
    Canny(cougarGrey, cougarEdge, 0.4*255, 0.5*255);
    imshow("Canny cougar edges with A: 0.4, B 0.5", cougarEdge);
    waitKey(0);
    return 0;
}


