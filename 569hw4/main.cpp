//
//  main.cpp
//  569hw4
//
//  Created by LiuHengyue on 11/13/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//

#include "569hw4.h"

int main(int argc, const char * argv[]) {
    // problem 1
    std::cout << "Hello, World!\n";
    const char* TRAINING="/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P1/Training.raw";
    imageData training(TRAINING,512,256,3);
    ShowImg("Traing.raw", training);
    return 0;
}
