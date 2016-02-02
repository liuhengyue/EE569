//
//  569hw4_p3.cpp
//  569 homework 4 problem 3
//  SIFT SURF K-MEANS BAG OF WORDS
//  Created by LiuHengyue on 11/24/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
////

#include "569hw4.h"
#define K_MEANS 8
/*************************************************************************
 * GetSIFT
 * Function: Compute SIFT features of an image,then draw the key points.
 * Input: Image Mat().
 * Output: SIFT descriptors.
 *************************************************************************/

Mat GetSIFT(Mat img){
    time_t start=clock();
    //define drawing panel
    Mat panel;
    //define SIFT variables
    Mat descriptors;
    vector<KeyPoint> keypoints;
    SIFT sift;
    //SIFT feature extraction
    sift.SIFT::operator()(img, noArray(), keypoints, descriptors);
    time_t end=clock();
    cout<<double(end-start)/CLOCKS_PER_SEC<<"s, keypoints number: "<<keypoints.size()<<endl;
    //for (int i=0; i<keypoints.size(); i++) {
    //    circle(panel, keypoints[i].pt, 1, Scalar(0,255,255),-1,8,0);
    //}
    drawKeypoints(img, keypoints, panel,Scalar(0,0,255));
    imshow("SIFT keypoints", panel);
    waitKey(0);
    return descriptors;
}

/*************************************************************************
 * GetSURF
 * Function: Compute SURF features of an image,then draw the key points.
 * Input: Image Mat().
 * Output: SIFT descriptors.
 *************************************************************************/

Mat GetSURF(Mat img){
    time_t start=clock();
    //define drawing panel
    Mat panel;
    //define SIFT variables
    Mat descriptors;
    vector<KeyPoint> keypoints,kpts;
    SURF surf;
    //SURF feature extraction
    surf.SURF::operator()(img, noArray(), keypoints, descriptors);
    //for (int i=0; i<keypoints.size(); i++) {
    //    circle(panel, keypoints[i].pt, 1, Scalar(255,255,0),-1,8,0);
    //}
    time_t end=clock();
    cout<<double(end-start)/CLOCKS_PER_SEC<<"s, keypoints number: "<<keypoints.size()<<endl;
    drawKeypoints(img, keypoints, panel,Scalar(0,0,255));
    imshow("SURF keypoints", panel);
    waitKey(0);
    return descriptors;
}

/*************************************************************************
 * MatchSIFT
 * Function: Compute SIFT features for the two images, and find the matching
 *           pairs of key points, then draw the matches.
 * Input: 2 images Mat().
 * Output: Image of matched pairs.
 *************************************************************************/

Mat MatchSIFT(Mat img1,Mat img2){
    //define drawing panel
    Mat panel;
    //define SIFT variables
    Mat descriptors1,descriptors2;
    vector<KeyPoint> keypoints1,keypoints2;
    SIFT sift;
    //SIFT feature extraction
    sift.SIFT::operator()(img1, noArray(), keypoints1, descriptors1);
    sift.SIFT::operator()(img2, noArray(), keypoints2, descriptors2);
    // matching descriptors
    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
    // drawing the results
    drawMatches(img1, keypoints1, img2, keypoints2, matches, panel,Scalar(255,255,0),Scalar(0,255,255));
    imshow("matches", panel);
    waitKey(0);
    return panel;
}

/*************************************************************************
 * CodewordsHis
 * Function: Compute codewords for feature vectors of one image, and compute
 *           the histogram of the codewords via codeBook.
 * Input: Feature vectors n*dim, codeBook k*dim. For SIFT, dim=128.
 * Output: Codewords histogram (normalized) 1*k.
 * Note: k is the k for the k-means, k=8 for this problem.
 *************************************************************************/

Mat CodewordsHis(Mat features, Mat codeBook){
    //histogram of codewords
    Mat his=Mat::zeros(1,codeBook.rows,features.type());
    //for each feature 20-d vector
    for (int i=0; i<features.rows; i++) {
        //store the nearest codeword
        double dis=INT_MAX;
        int code=0;
        //compute the distance to each codeword
        for (int row=0; row<codeBook.rows; row++) {
            double curDis=norm(features.row(i), codeBook.row(row));
            //find the nearest neighbor and assign label
            if (curDis<dis) {
                dis=curDis;
                code=row;
            }
            
        }
        his.at<float>(0,code)+=1;
    }
    //normalized histogram
    for (int k=0; k<his.cols; k++) {
        his.at<float>(0,k)/=features.rows;
    }
    return his;
}

int main(int argc, const char * argv[]) {
    //define images matrices
    Mat bus=Raw2Mat("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P3/Bus.raw", 400, 300, 3);
    Mat school_bus_1=Raw2Mat("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P3/School_bus1.raw", 374, 200, 3);
    Mat school_bus_2=Raw2Mat("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P3/School_bus2.raw", 356, 200, 3);
    Mat sedan=Raw2Mat("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P3/Sedan.raw", 400, 233, 3);
    //(a)
    
    cout<<"bus SIFT time: ";
    GetSIFT(bus);
    cout<<"bus SURF time: ";
    GetSURF(bus);
    cout<<endl;
    cout<<"sedan SIFT time: ";
    GetSIFT(sedan);
    cout<<"sedan SURF time: ";
    GetSURF(sedan);

    //(b)
    MatchSIFT(school_bus_2,school_bus_1);
    MatchSIFT(bus,school_bus_1);
    MatchSIFT(sedan,school_bus_1);
    
    //(c)
    //get SIFT for each image
    Mat bus_SIFT=GetSIFT(bus);
    Mat sedan_SIFT=GetSIFT(sedan);
    Mat schoolbus1_SIFT=GetSIFT(school_bus_1);
    Mat schoolbus2_SIFT=GetSIFT(school_bus_2);
    
    /* my implementation
    // bag of words
    cout<<"conducting k-means..."<<endl;
    //define features and codebook,codebook should be 8*128
    Mat featureData,labels,codeBook;
    //construct features from 3 images
    featureData.push_back(bus_SIFT);
    featureData.push_back(sedan_SIFT);
    featureData.push_back(schoolbus1_SIFT);
    //k-means
    kmeans(featureData, K_MEANS, labels,
           TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 50, 1.0),
           3, KMEANS_PP_CENTERS, codeBook);
    // compute histograms for each image
    CodewordsHis(bus_SIFT, codeBook);
    CodewordsHis(sedan_SIFT, codeBook);
    CodewordsHis(schoolbus1_SIFT, codeBook);
    CodewordsHis(schoolbus2_SIFT, codeBook);
    */
    //using BOW trainer
    //construct features from 3 images
    Mat featureData;
    featureData.push_back(bus_SIFT);
    featureData.push_back(sedan_SIFT);
    featureData.push_back(schoolbus1_SIFT);
    //kmeans k=8
    BOWKMeansTrainer bow(8);
    bow.add(featureData);
    Mat dic=bow.cluster();
    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    //create Sift feature point extracter
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    //create Sift descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
    //create BoF (or BoW) descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor,matcher);
    bowDE.setVocabulary(dic);
    Mat descriptors;
    vector<KeyPoint> bus_kps,sedan_kps,school_bus1_kps,school_bus2_kps;
    SIFT sift;
    //SIFT feature extraction
    sift.SIFT::operator()(bus, noArray(), bus_kps, descriptors);
    sift.SIFT::operator()(sedan, noArray(), sedan_kps, descriptors);
    sift.SIFT::operator()(school_bus_1, noArray(), school_bus1_kps, descriptors);
    sift.SIFT::operator()(school_bus_2, noArray(), school_bus2_kps, descriptors);
    //compute bow
    Mat bus_bowD,sedan_bowD,schoolbus1_bowD,schoolbus2_bowD;
    bowDE.compute(bus, bus_kps, bus_bowD);
    bowDE.compute(sedan, sedan_kps, sedan_bowD);
    bowDE.compute(school_bus_1, school_bus1_kps, schoolbus1_bowD);
    bowDE.compute(school_bus_2, school_bus2_kps, schoolbus2_bowD);
    //show histograms
    cout<<"bus BOW descriptors: "<<endl<<bus_bowD<<endl;
    cout<<"sedan BOW descriptors: "<<endl<<sedan_bowD<<endl;
    cout<<"school bus 1 BOW descriptors: "<<endl<<schoolbus1_bowD<<endl;
    cout<<"school bus 2 BOW descriptors: "<<endl<<schoolbus2_bowD<<endl;
    cout<<"dictionary:"<<endl<<dic<<endl;
    return 0;
}