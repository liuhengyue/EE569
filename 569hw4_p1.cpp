//
//  main.cpp
//  569hw4
//
//  Created by LiuHengyue on 11/13/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
////

#include "569hw4.h"

//for storing image files' paths
int testIndex=0;

    //set data for each character
void character::setSymbol(imageData symbol){
        _symbol=symbol;
    }
    void character::setShape(char s){
        _shape=s;
    }
    bool character::empty(){
        if (_symbol._dataPtr==0) {
            return true;
        }
    
        else{
            //cout<<_symbol._length<<" "<<_symbol._width<<endl;
            for (int j=0; j<_symbol._length; j++) {
                for (int i=0; i<_symbol._width; i++) {
                    //if has labeled object,the symbol is not empty
                    if (_symbol._dataPtr[i][j][0]==0) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    //display geometric features
    void character::showFeatures(){
        cout<<"Shape is: "<<_shape<<endl;
        cout<<"Aspect ratio is: "<<_aspect<<endl;
        cout<<"Normalized Area A0 is: "<<_A0<<endl;
        cout<<"Normalized Perimeter P0 is: "<<_P0<<endl;
        cout<<"Circularity is: "<<_circularity<<endl;
        cout<<"Euler Number is: "<<_EulerNo<<endl;
        cout<<"Horizontal centroid is: "<<_xCog<<endl;
        cout<<"Vertical centroid is: "<<_yCog<<endl;
        cout<<"Stroke cut is: "<<_strokeCut<<endl;
        cout<<"Symmetry is: "<<_symmetry<<endl;

        }
    void character::saveFeatures(const char* path){
        //save features into file
        ofstream outFile;
        outFile.open(path,std::ofstream::out | std::ofstream::app);
        outFile<<fixed<<setprecision(4)<<"    "<<_shape<<"    "<<_aspect<<"    "<<_A0<<"    "<<_P0<<"      "<<_circularity<<"    "<<_EulerNo<<"      "<<_xCog<<"      "<<_yCog<<"    "<<_strokeCut<<"    "<<_symmetry<<endl;
        outFile.close();
    }
    

//////////////////////////////////////////////////////////
//    pre-processing functions for OCR system
//////////////////////////////////////////////////////////

/*************************************************************************
 * Color2Gray()
 * Function: This function converts color image into gray-scale image.
 * Input: Original color image.
 * Output: Gray scale image.
 *************************************************************************/

imageData Color2Gray(imageData img){
    imageData grayImg(img._width,img._length,1);
    int i,j;
    for(i=0;i<img._width;i++){//array row
        for(j=0;j<img._length;j++){//coloumn
            //rgb to gray-scale
            grayImg._dataPtr[i][j][0]=0.2126*img._dataPtr[i][j][0]+0.7152*img._dataPtr[i][j][1]+0.0722*img._dataPtr[i][j][2];
            }
        }
    //apply Gaussian filter with 8 neighbors
    imageData meanImg(img._width,img._length,1);
    for(i=0;i<img._width;i++){//array row
        for(j=0;j<img._length;j++){//coloumn
            if (i==0 || j==0 || i==img._width-1 || j==img._length-1) {
                meanImg._dataPtr[i][j][0]=grayImg._dataPtr[i][j][0];
            }
            //apply filter
            else{
                meanImg._dataPtr[i][j][0]=float(4*grayImg._dataPtr[i][j][0]+2*grayImg._dataPtr[i+1][j][0]+grayImg._dataPtr[i+1][j-1][0]+2*grayImg._dataPtr[i][j-1][0]+grayImg._dataPtr[i-1][j-1][0]+2*grayImg._dataPtr[i-1][j][0]+grayImg._dataPtr[i-1][j+1][0]+2*grayImg._dataPtr[i][j+1][0]+grayImg._dataPtr[i+1][j+1][0])/16;
            }
        }
    }
    return meanImg;
    
}
/*************************************************************************
 * GetHist()
 * Function: This function computes the histogram of the gray-scale image.
 * Input: Gray-scale image.
 * Output: Corresponding histogram.
 *************************************************************************/

int* GetHist(imageData img){
    int* histogram=new int[256];
    //initialize histogram all zeros
    for(int k=0;k<256;k++){
        histogram[k]=0;
    }
    for(int i=0;i<img._width;i++){//array row
        for(int j=0;j<img._length;j++){//coloumn
            //check gray-scale value and set histogram
            histogram[int(img._dataPtr[i][j][0])]++;
            
        }
    }
    for(int k=0;k<256;k++){
        //cout<<histogram[k]<<endl;
    }

    return histogram;
}
/*************************************************************************
 * BrightnessEnhancement()
 * Function: This function increases the intensity values of original image.
 * Input: Original gray-scale image.
 * Output: Brighter gray-scale image.
 *************************************************************************/

imageData BrightnessEnhancement(imageData img){
    imageData copy=img.copy();
    float avgIntesity=0;
    for(int i=0;i<copy._width;i++){//array row
        for(int j=0;j<copy._length;j++){//coloumn
            avgIntesity+=copy._dataPtr[i][j][0];
        }
    }
    avgIntesity/=copy._width*copy._length;
    if(avgIntesity<128){
        int increment=100;
        //increase intensity
        for(int i=0;i<copy._width;i++){//array row
            for(int j=0;j<copy._length;j++){//coloumn
                if (copy._dataPtr[i][j][0]+increment>255) {
                    copy._dataPtr[i][j][0]=255;
                }
                else{
                    copy._dataPtr[i][j][0]+=increment;
                }
            }
        }
    }
    //cout<<"average intensity: "<<avgIntesity<<endl;
    return copy;
    
}
/*************************************************************************
 * TransferFunctionEqualization()
 * Function: This function calculates the transfer function of the image,
 *           and apply the equalization.
 * Input: Original gray-scale image.
 * Output: Equalized gray-scale image.
 *************************************************************************/

imageData TransferFunctionEqualization(imageData img){
    imageData copy=img.copy();
    int* his=GetHist(copy);
    float tranferFunction[256];
    //check the number of small slopes of the transfer function shape
    int smoothNo=0;
    //calculate transfer function
    for(int k=0;k<256;k++){
        tranferFunction[k]=0;
        for (int l=0; l<k+1; l++) {
            tranferFunction[k]+=his[l];
        }
        tranferFunction[k]=float(tranferFunction[k])/(copy._width*copy._length);
        if (k>0 && tranferFunction[k]/tranferFunction[k-1]>=1 && tranferFunction[k]/tranferFunction[k-1]<1.1) {
            smoothNo++;
        }
        
    }
    //cout<<smoothNo<<endl;
    //assign equalization values if histogram is not smooth
    if(smoothNo<255*0.8){
        for(int i=0;i<copy._width;i++){//array row
            for(int j=0;j<copy._length;j++){//coloumn
                //check gray-scale value and equalize
                for (int l=0; l<256; l++) {
                    if(copy._dataPtr[i][j][0]==l){
                        copy._dataPtr[i][j][0]=255*tranferFunction[l];
                    }
                }
                
            }
        }
    }
    return copy;
    
}
/*************************************************************************
 * Binarization()
 * Function: This function converts gray-scale image into binary image.
 * Input: Original gray-scale image.
 * Output: Binary black/white image.
 *************************************************************************/

imageData Binarization(imageData img){
    //construct binarized image
    imageData binaryImg=img.createData();
    //get histogram (part of dual maximum method)
    int* his=GetHist(img);
    int maxValue=INT_MIN,secondMaxValue=INT_MIN;
    for(int i=0;i<256;i++){
        if(his[i]>maxValue){
            secondMaxValue=maxValue;
            maxValue=his[i];
        }
        if (his[i]<maxValue && his[i]>secondMaxValue) {
            secondMaxValue=his[i];
        }
    }
    //end of dual maximum method
    
    //naive binarization by setting threshold=128
    for(int i=0;i<img._width;i++){//array row
        for(int j=0;j<img._length;j++){//coloumn
            //check gray-scale value and equalize
            if (img._dataPtr[i][j][0]<128) {
                binaryImg._dataPtr[i][j][0]=0;
            }
            else{
                binaryImg._dataPtr[i][j][0]=255;

            }
    
        }
    }
    //isolate dot noise removal (hole filling)
    int xshift[4]={1,0,-1,0};
    int yshift[8]={0,1,0,-1};

    for(int i=0;i<binaryImg._width;i++){//array row
        for(int j=0;j<binaryImg._length;j++){//coloumn
            if (i==0 || j==0 || i==binaryImg._width-1 || j==binaryImg._length-1) {
                binaryImg._dataPtr[i][j][0]=255;
            }
            else{
            //check 4-connected neighbors
            bool noise=true;
            for (int k=0; k<4; k++) {
                //if has same value, then not isolate dot
                if (binaryImg._dataPtr[i+xshift[k]][j+yshift[k]][0]==binaryImg._dataPtr[i][j][0]) {
                    noise=false;
                    break;
                }
            }
            //remove noise
            if (noise==true) {
                binaryImg._dataPtr[i][j][0]=255-binaryImg._dataPtr[i][j][0];
            }
        }
        }
    }
    return binaryImg;
    
}
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

imageData PreProcess(imageData img){
    //return the binary image with pre-processing
    return Binarization(TransferFunctionEqualization(BrightnessEnhancement(Color2Gray(img))));
}

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

bool InVector(int label,vector<int> list){
    if (find(list.begin(), list.end(), label)==list.end()) {
        return false;
    }
    else{
        return true;
    }
}

/*************************************************************************
 * MinLabel()
 * Function: This function generates the smallest label for certain table.
 * Input: certain label equivalence table.
 * Output: smallest label.
 *************************************************************************/

int MinLabel(vector<int> list){
    int smallestValue=INT_MAX;
    for (int i=0; i<list.size(); i++) {
        if (list[i]<smallestValue) {
            smallestValue=list[i];
        }
    }
    return smallestValue;
}
/*************************************************************************
 * BBSymbol()
 * Function: This function generates particular symbol with corresponding 
 *           bounding box.
 * Input: Labeled image, particular label, original image width and length.
 * Output: One symbol.
 *************************************************************************/

character BBSymbol(int** labels,int label, int width, int length){
    //bounding box boundaries
    int left=width,right=0,top=length,bottom=0;
    //find the bounding box
    for(int j=0;j<length;j++){//column
        for(int i=0;i<width;i++){//row
            //for the symbol with particular label
            if (labels[i][j]==label) {
                //check left
                if (labels[i-1][j]==0 && i<left) {
                    left=i;
                }
                //check right
                if (labels[i+1][j]==0 && i>right) {
                    right=i;
                }
                //check top
                if (labels[i][j-1]==0 && j<top) {
                    top=j;
                }
                //check bottom
                if (labels[i][j+1]==0 && j>bottom) {
                    bottom=j;
                }

            }
        }
    }
    
    int BBWidth=right-left+3;
    int BBLength=bottom-top+3;
    //cout<<float(BBWidth-3)/(BBLength-3)<<endl;
    //some symbols may be not the character, throw them away
    if((BBWidth>10 || BBLength>10) && BBWidth<width*0.6 && BBLength<length*0.6 && ((float(BBWidth-2)/(BBLength-2)<1 && float(BBWidth-2)/(BBLength-2)>0.3 )|| (float(BBWidth-2)/(BBLength-2)<0.25))){
        character charData;
        //construct BB symbol. Note: add white boundaries to the symbol image
        imageData symbol(right-left+3, bottom-top+3, 1);
        //extract symbol
        for(int j=top-1;j<bottom+2;j++){//column
            for(int i=left-1;i<right+2;i++){//row
                if (labels[i][j]==label) {
                    symbol._dataPtr[i-left+1][j-top+1][0]=0;
                    //cout<<labels[i][j]<<" ";
                    
                }
                else{
                    symbol._dataPtr[i-left+1][j-top+1][0]=255;
                }
                
            }
            //cout<<endl;
        }
        stringstream sts;
        sts<<"/Users/liuhengyue/Desktop/OCR results/"<<testIndex<<"_"<<symbol._width<<symbol._length<<".jpg";
        imwrite(sts.str(), ImageData2Mat(symbol));

        charData.setSymbol(symbol);
        return charData;
        //ShowImg("test", symbol);
    }
    else{
        return character();
    }

}
/*************************************************************************
 * SymbolSegment()
 * Function: This function first label the image into several segmentations
 *           using the connected component labeling method provided in 
 *           discussion 9; then find each symbol's ROI and extract each
 *           symbol with bounding box.
 * Input: Binary image(after pre-processing).
 * Output: A set of symbols.
 *************************************************************************/

vector<character> SymbolSegment(imageData img){
    imageData copy=img.copy();
    //labeled image
    imageData labelImg(copy._width,copy._length,1);
    //symbols
    vector<character> chars;
    //create labels for each pixel
    int** labels;
    //neighbor shift
    int x[4]={1,0,-1,-1};
    int y[4]={-1,-1,-1,0};
    //label equivalence table
    vector<vector<int>> table;
    int curLabel=1;
    //initialize labels
    labels=new int*[copy._width];
    for(int i=0;i<copy._width;i++){//row
        labels[i]=new int[copy._length];
            for(int j=0;j<copy._length;j++){//column
                labels[i][j]=0;
        }
    }
    //first pass
    //scan from left to right first
    for(int j=0;j<copy._length;j++){//column
        for(int i=0;i<copy._width;i++){//row
            //cout<<int(copy._dataPtr[i][j][0])<<endl;
            //label background to 0, and set boudaries label as background
            if (copy._dataPtr[i][j][0]==255 || i==0 || i==copy._width-1 || j==0) {
                labels[i][j]=0;
                }
            //pixel is object
            else{
                //not at boundaries
                //all neighbors are background
                if (copy._dataPtr[i-1][j][0]==255 && copy._dataPtr[i-1][j-1][0]==255 && copy._dataPtr[i][j-1][0]==255 && copy._dataPtr[i+1][j-1][0]==255) {
                    
                    labels[i][j]=curLabel;
                    vector<int> curItem;
                    //store label
                    curItem.push_back(curLabel);
                    table.push_back(curItem);
                    curLabel++;
                    
                }
                //some neighbors have been labeled
                else{
                    //cout<<"yes";
                    //find the smallest label
                    int minLabel=INT_MAX;
                    //check neighbors
                    for (int k=0; k<4; k++) {
                        //not background neighbor
                        if (labels[i+x[k]][j+y[k]]>0 && labels[i+x[k]][j+y[k]]<minLabel) {
                            minLabel=labels[i+x[k]][j+y[k]];
                        }
                    }
                    //isolate dot
                    if(minLabel==INT_MAX){
                        minLabel=0;
                    }

                    //assign the smallest label to current pixel
                    labels[i][j]=minLabel;
                    //update label equivalence table
                    //compare each pair of neighbors
                    for (int k=0; k<4; k++) {
                        for (int n=0; n<4; n++) {
                            //check 2 different label list
                            if (x[k]!=x[n] && y[k]!=y[n]) {
                                if (labels[i+x[k]][j+y[k]]>0 && labels[i+x[n]][j+y[n]]>0 && (!InVector(labels[i+x[n]][j+y[n]], table[labels[i+x[k]][j+y[k]]-1]) || !InVector(labels[i+x[k]][j+y[k]], table[labels[i+x[n]][j+y[n]]-1]))) {
                                    //put all elements in one list but not in the other list to the other list
                                    for (int w=0; w<table[labels[i+x[n]][j+y[n]]-1].size(); w++) {
                                        if(!InVector(table[labels[i+x[n]][j+y[n]]-1][w], table[labels[i+x[k]][j+y[k]]-1])){
                                            table[labels[i+x[k]][j+y[k]]-1].push_back(table[labels[i+x[n]][j+y[n]]-1][w]);
                                        }
                                    }
                                    for (int w=0; w<table[labels[i+x[k]][j+y[k]]-1].size(); w++) {
                                        if(!InVector(table[labels[i+x[k]][j+y[k]]-1][w], table[labels[i+x[n]][j+y[n]]-1])){
                                            table[labels[i+x[n]][j+y[n]]-1].push_back(table[labels[i+x[k]][j+y[k]]-1][w]);
                                        }
                                    }
                                    
                                }
                                
                            }
                        }
                    }
                }

            }
                        //cout<<labels[i][j]<<" ";
        }
    }
    //cout<<"First pass done"<<endl;
    //record the final labels
    vector<int> finalLabels;
    //second pass
    for(int j=0;j<copy._length;j++){//column
        for(int i=0;i<copy._width;i++){//row
            if (labels[i][j]!=0) {
                //change labels to the smallest label
                labels[i][j]=MinLabel(table[labels[i][j]-1]);
                //store the final labels
                if (!InVector(MinLabel(table[labels[i][j]-1]), finalLabels)) {
                    finalLabels.push_back(MinLabel(table[labels[i][j]-1]));
                }
            }
           
        }
    }
    //for (int i=0; i<finalLabels.size(); i++) {
    //    cout<<finalLabels[i]<<" ";
    //}
    //assign labels to create label image
    
    for(int j=0;j<copy._length;j++){//column
        for(int i=0;i<copy._width;i++){//row
            labelImg._dataPtr[i][j][0]=labels[i][j];
            //cout<<int(labelImg._dataPtr[i][j][0])<<endl;
        }
        //cout<<endl;
    }
    for (int s=0; s<finalLabels.size(); s++) {
        //magic?
        //cout<<endl;
        character candidate=BBSymbol(labels, finalLabels[s],copy._width,copy._length);
        //cout<<candidate.empty()<<endl;
        //check if the candidate symbol is empty
        if (!candidate.empty()) {
            chars.push_back(candidate);
        }
    }
    return chars;
}

//////////////////////////////////////////////////////////
//    features extraction
//////////////////////////////////////////////////////////

/*************************************************************************
 * SpatialMoment()
 * Function: This function calculates the scaled spatial central moment of
 *           each symbol.
 * Input: symbol image, horizontal order m, vertical order n, Centroid x, 
 *        Centroid y.
 * Output: Float Moment value.
 *************************************************************************/

float SpatialMoment(imageData img, int m, int n,float xCog,float yCog){
    imageData copy=img.copy();
    //spatial moment
    float moment=0;
    for(int j=0;j<copy._length;j++){//row
        for(int i=0;i<copy._width;i++){//column
            //change to Cartisian coordinates
            float x=i+0.5;
            float y=copy._length-0.5-j;
            if (copy._dataPtr[i][j][0]==0) {
                moment+=pow(x-xCog, m)*pow(y-yCog, n);
                //cout<<pow(x-xCog, m)<<" "<<x-xCog<<endl;
            }
        }
    }
    moment/=pow(copy._width, m)*pow(copy._length, n);
    //cout<<"M("<<m<<", "<<n<<") Spatial moment is: "<<moment<<endl;
    return moment;
}

/*************************************************************************
 * StrokeCut()
 * Function: This function counts the number of the cuts of one horizontal
 *           line and one vertical line.
 * Input: one character (candidate).
 * Output: number of cuts.
 *************************************************************************/

int StrokeCut(imageData sample){
    //thining the image
    imageData thinResult=Morphing(sample, 2, 1);
    //ShowImg("fefe", thinResult);
    
    //use multiple test times to get the true cuts number
    vector<int> multiResult;
    //horizontal line
    int y=0;
    for (int n=0; n<thinResult._length; n++) {
        //vertical line
        int x=0;
        for (int m=0; m<thinResult._width; m++) {
            //cuts count
            int cuts=0;
            for (int i=0; i<sample._width; i++) {
                if(thinResult._dataPtr[i][y][0]==255){
                    cuts++;
                    //avoid thick lines
                    i++;
                }
            }
            for (int j=0; j<sample._length; j++) {
                if(thinResult._dataPtr[x][j][0]==255){
                    cuts++;
                    //avoid thick lines
                    j++;
                }
            }
            multiResult.push_back(cuts);
            x++;
        }
        y++;
    }
    //cout<<multiResult.size()<<endl;
    //find most frequent cuts
    int cutCount[multiResult.size()];
    for (int k=0; k<multiResult.size(); k++) {
        cutCount[k]=0;
        for (int s=0; s<multiResult.size(); s++) {
            if (multiResult[k]==multiResult[s]) {
                cutCount[k]++;
            }
        }
    }
    //cout<<"yes"<<endl;
    int mostNo=0;
    int index=0;
    for (int k=0; k<multiResult.size(); k++) {
        if (cutCount[k]>mostNo) {
            mostNo=cutCount[k];
            index=k;
        }
    }
    //cout<<"yes"<<endl;
    //cout<< multiResult[index]<<endl;
    return multiResult[index];
    }

float Symmetry(imageData sample){
    imageData copy=sample.copy();
    int hitNumber=1;
    float gotMatch=0;
    for(int shift=-10;shift<11;shift+=2){
        int curHit=0;
        float curGotMatch=0;
        for (int j=0; j<copy._length; j++) {
            if ((copy._width+shift)/2<copy._width) {
                for (int i=0; i<(copy._width+shift)/2; i++) {
                    if (copy._dataPtr[i][j][0]==0) {
                        curHit++;
                        if(copy._width-i+shift<copy._width){
                            if (copy._dataPtr[copy._width-i+shift][j][0]==0) {
                                curGotMatch++;
                            }
                        }
                    }
                }
            }

        }
        if (curGotMatch/curHit>gotMatch/hitNumber) {
            gotMatch=curGotMatch;
            hitNumber=curHit;
        }
    }
    return gotMatch/hitNumber;
}


/* Bit Quads Patterns */

int Q0[4]={0,0,0,0};
int Q1[4][4]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
int Q2[4][4]={1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1};
int Q3[4][4]={1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,1};
int Q4[4]={1,1,1,1};
int QD[2][4]={1,0,1,0,0,1,0,1};

//Bit Quads shift
int x[4]={0,1,1,0};
int y[4]={0,0,1,1};

/*************************************************************************
 * setFeatures()
 * Function: This function checks each Bit Quad 4*4 pixels through the 
 *           symbol image, then calculates the Area and Perimeter geometric
 *           features of the symbol; then computes the scaled spatial central
 *           moment.
 * Input: Symbol image.
 * Output: Related geometric symbols.
 *************************************************************************/
void character::setFeatures(){
    imageData copy=_symbol.copy();
    /* compute area and perimeter */
    //bounding box area and perimeter
    int BBArea=(copy._width-2)*(copy._length-2);
    int BBPeri=2*(copy._width+copy._length-4);
    //pattern match numbers
    int numQ0=0,numQ1=0,numQ2=0,numQ3=0,numQ4=0,numQD=0;
    for(int j=0;j<copy._length-1;j++){//row
        for(int i=0;i<copy._width-1;i++){//column
            //Q0
            bool match=true;
            for (int w=0; w<4; w++) {
                if (1-copy._dataPtr[i+x[w]][j+y[w]][0]/255 != Q0[w]) {
                    match=false;
                    break;
                }
            }
            numQ0+=match;
            //Q1
            match=false;
            for(int k=0;k<4;k++){
                int subMatch=true;
                for (int w=0; w<4; w++) {
                    if (1-copy._dataPtr[i+x[w]][j+y[w]][0]/255 != Q1[k][w]) {
                        subMatch=false;
                        break;
                    }
                }
                match=match || subMatch;
            }
            numQ1+=match;
            //Q2
            match=false;
            for(int k=0;k<4;k++){
                int subMatch=true;
                for (int w=0; w<4; w++) {
                    if (1-copy._dataPtr[i+x[w]][j+y[w]][0]/255 != Q2[k][w]) {
                        subMatch=false;
                        break;
                    }
                }
                match=match || subMatch;
            }
            numQ2+=match;
            //Q3
            match=false;
            for(int k=0;k<4;k++){
                int subMatch=true;
                for (int w=0; w<4; w++) {
                    if (1-copy._dataPtr[i+x[w]][j+y[w]][0]/255 != Q3[k][w]) {
                        subMatch=false;
                        break;
                    }
                }
                match=match || subMatch;
            }
            numQ3+=match;
            //Q4
            match=true;
            for (int w=0; w<4; w++) {
                if (1-copy._dataPtr[i+x[w]][j+y[w]][0]/255 != Q4[w]) {
                    match=false;
                    break;
                }
            }
            numQ4+=match;
            //QD
            match=false;
            for(int k=0;k<2;k++){
                int subMatch=true;
                for (int w=0; w<4; w++) {
                    if (1-copy._dataPtr[i+x[w]][j+y[w]][0]/255 != QD[k][w]) {
                        subMatch=false;
                        break;
                    }
                }
                match=match || subMatch;
            }
            numQD+=match;


        }
        //cout<<endl;
    }
    //cout<<numQ0<<" "<<numQ1<<" "<<numQ2<<" "<<numQ3<<" "<<numQ4<<" "<<numQD<<endl;
    /* features */
    _EulerNo=(numQ1-numQ3-2*numQD)/4;
    _A0=float(numQ1+2*numQ2+3*numQ3+4*numQ4+2*numQD)/4/BBArea;
    _P0=float(numQ1+numQ2+numQ3+2*numQD)/BBPeri;
    _circularity=M_PI*(numQ1+2*numQ2+3*numQ3+4*numQ4+2*numQD)/pow((numQ1+numQ2+numQ3+2*numQD), 2);
    //cout<<float(numQ1+2*numQ2+3*numQ3+4*numQ4+2*numQD)/4<<" "<<float(numQ1+numQ2+numQ3+2*numQD)<<endl;
    _xCog=SpatialMoment(copy, 1, 0,0,0)/SpatialMoment(copy, 0, 0,0,0);
    _yCog=SpatialMoment(copy, 0, 1,0,0)/SpatialMoment(copy, 0, 0,0,0);
    _aspect=float(_symbol._width-2)/(_symbol._length-2);
    _strokeCut=StrokeCut(copy);
    _symmetry=Symmetry(copy);
    

    
}
char OCRDecisionTree(character candidate){
    char shape;
    if (candidate._EulerNo==-1) {
        shape='8';
    }
    //PD0469
    else if (candidate._EulerNo==0){
        //4 6 D
        if (candidate._yCog<0.5) {
            if (candidate._xCog<0.5) {
                if (candidate._aspect>0.8) {
                    shape='D';
                }
                else{
                    shape='6';
                }
            }
            else{
                shape='4';
            }
        }
        //D 0 9
        else if (candidate._yCog>=0.5 && candidate._yCog<0.59){
            //D 0
            if (candidate._strokeCut==4) {
                if (candidate._xCog<0.48) {
                    shape='D';
                }
                else{
                    shape='0';
                }
            }
            
            else{
                shape='9';
            }
        }
        //P
        else{
            shape='P';
        }
    }
    //SELIMT12357
    else if(candidate._EulerNo==1){
        if (candidate._strokeCut==1) {
            shape='I';
        }
        //L T 1 7
        else if (candidate._strokeCut==2){
            if (candidate._aspect<0.5) {
                shape='1';
            }
            //L T 7
            else{
                if (candidate._yCog<0.5) {
                    shape='L';
                }
                else{
                    if (candidate._P0<1.1) {
                        shape='T';
                    }
                    else{
                        shape='7';
                    }
                }
            }
        }
        //S E M 2 3 5 7
        else{
            //3 M
            if (candidate._symmetry>0.9) {
                if (candidate._xCog>0.55) {
                    shape='3';
                }
                else{
                    shape='M';
                }
            }
            //S E 2 3 5 7
            else{
                if (candidate._xCog<0.45) {
                    shape='E';
                }
                else if(candidate._xCog>0.55){
                    shape='3';
                }
                //S 2 5 7
                else{
                    if (candidate._yCog<0.48) {
                        shape='2';
                    }
                    
                    else if (candidate._yCog>=0.48 && candidate._yCog<0.5){
                        shape='S';
                    }
                    
                    else if (candidate._yCog>=0.5 && candidate._yCog<0.6){
                        shape='5';
                    }
                    else{
                        shape='7';
                    }
                }
            }
        }
    }
    else{
        cout<<"unknown character."<<endl;
        exit(0);
    }
    return shape;
}
void Training(const char* trainPath){
    //read raw file
    const char* TRAINING="/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P1/Training.raw";
    imageData training(TRAINING,512,256,3);
    //ShowImg("original image", training);
    imageData grayTraining=Color2Gray(training);
    //define shape symbols knowledge
    const char* shapes="SPEDLIMT0123456789";
    //get symbols, order is SPEDLIMT0123456789
    vector<character> chars= SymbolSegment(Binarization(grayTraining));
    ofstream head;
    head.open(trainPath,std::ofstream::out | std::ofstream::trunc);
    head<<fixed<<"  Shape  AspectRatio  Area  Perimeter  Circularity  EulerNo.  x_COG  y_COG  StrokeCut Symmetry"<<endl;
    head.close();
    //calculate geometric features
    for (int i=0;i<chars.size();i++) {
        
        chars[i].setShape(shapes[i]);
        //ShowImg("symbol", chars[i]._symbol);
        //ShowImg("thining symbol", Morphing(chars[i]._symbol, 2, 1) );
        chars[i].setFeatures();
        chars[i].saveFeatures(trainPath);
        
        //chars[i].showFeatures();
    }
    //training
    

}
void Testing(imageData testImg,const char* testPath){
    imageData processImg=PreProcess(testImg);
    stringstream sts;
    sts<<"/Users/liuhengyue/Desktop/OCR results/"<<testIndex<<"_"<<testPath[26]<<testPath[27]<<testPath[28]<<testPath[29]<<testPath[30]<<".jpg";
    imwrite(sts.str(), ImageData2Mat(processImg));
    //ShowImg("test image after pre-processing",processImg);
    vector<character> segments= SymbolSegment(processImg);
    //create feature file
    ofstream head;
    head.open(testPath,std::ofstream::out | std::ofstream::trunc);
    head<<fixed<<"  Shape  AspectRatio  Area  Perimeter  Circularity  EulerNo.  x_COG  y_COG  StrokeCut Symmetry"<<endl;
    head.close();
    //ideal1: ESEDPLIMIT07
    //ideal2: SPEEDLIIMT31
    //night: SDPEETIMIL591
    //train SPEDLIMT0123456789
    for (int i=0;i<segments.size();i++) {
        //segments[i].setShape(shape[i]);
        segments[i].setFeatures();
        
        //ShowImg("test", segments[i]._symbol);
        //segments[i].showFeatures();

        
    }
    cout<<"OCR predictions: ";
    for (int i=0;i<segments.size();i++) {
        char decision=OCRDecisionTree(segments[i]);
        cout<<decision<<" ";
        segments[i].setShape(decision);
        segments[i].saveFeatures(testPath);
    }
    cout<<endl;
    testIndex++;

}

int main(int argc, const char * argv[]) {
    //problem 1
    cout<<"Problem 1:"<<endl<<"Reading raw images...";
    const char* IDEAL1="/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P1/Test_Ideal1.raw";
    const char* IDEAL2="/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P1/Test_Ideal2.raw";
    const char* TESTNIGHT="/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P1/Test_night.raw";
    const char* TESTSHADE="/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P1/Test_shade.raw";
    const char* TRAINING="/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P1/Training.raw";
    imageData training(TRAINING,512,256,3);
    imageData ideal1(IDEAL1,620,759,3);
    imageData ideal2(IDEAL2,250,400,3);
    imageData testNight(TESTNIGHT,267,360,3);
    imageData testShade(TESTSHADE,215,361,3);
    
    cout <<"Done!"<<endl<<"OCR training...";
    Training("/Users/liuhengyue/Desktop/trainData.txt");
    cout<<"Done!"<<endl<<"Testing..."<<endl<<endl;
    ;
    const char* shape="SPEDLIMT0123456789";
    
    
    //Testing(training,"/Users/liuhengyue/Desktop/traintestData.txt");
    //cout<<"Correct shapes: "<<shape<<" "<<endl<<endl;
    //4 testing images
    
    shape="SDPEETIMIL591";
    Testing(testNight,"/Users/liuhengyue/Desktop/nightData.txt");
    cout<<"Correct shapes: "<<shape<<" "<<endl<<endl;
    
    shape="ESEDPLIMIT07";
    Testing(ideal1,"/Users/liuhengyue/Desktop/ideal1Data.txt");
    cout<<"Correct shapes: "<<shape<<" "<<endl<<endl;
    
    shape="SPEEDLIIMT31";
    Testing(ideal2,"/Users/liuhengyue/Desktop/ideal2Data.txt");
    cout<<"Correct shapes: "<<shape<<" "<<endl<<endl;
    
    shape="DEEPSTMIL52ROUYEDESP";
     
    Testing(testShade,"/Users/liuhengyue/Desktop/shadeData.txt");
    cout<<"Correct shapes: "<<shape<<" "<<endl<<endl;
    
    //ShowImg("step", Color2Gray(testShade));
    //ShowImg("step", BrightnessEnhancement(Color2Gray(testShade)));
    //ShowImg("step", TransferFunctionEqualization(BrightnessEnhancement(Color2Gray(testShade))));
    //problem 2 convert image format
    /*
    imageData spine("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P2/spine.raw",600,390,1);
    imageData brain("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P2/brain.raw",637,713,1);
    imageData coronary("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P2/coronary.raw",481,446,1);
    imageData blood_cells("/Users/liuhengyue/Google Drive/EE569/hw4/HW4_images/P2/blood_cells.raw",836,672,1);
    imwrite("/Users/liuhengyue/Desktop/569hw4/p2/spine.jpg", ImageData2Mat(spine));
    imwrite("/Users/liuhengyue/Desktop/569hw4/p2/brain.jpg", ImageData2Mat(brain));
    imwrite("/Users/liuhengyue/Desktop/569hw4/p2/coronary.jpg", ImageData2Mat(coronary));
    imwrite("/Users/liuhengyue/Desktop/569hw4/p2/blood_cells.jpg", ImageData2Mat(blood_cells));
     */
    return 0;
}
