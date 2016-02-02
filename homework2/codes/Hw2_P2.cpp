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
