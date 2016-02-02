//
//  nms.h
//  HW2P2
//
//  Created by LiuHengyue on 10/11/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//

#ifndef nms_h
#define nms_h
#include <stdio.h>
#include <iostream>
#include <limits>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
void nonMaximaSuppression(const Mat& src, const int sz, Mat& dst, const Mat mask);


#endif /* nms_h */
