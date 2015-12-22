//
//  EE569_HW3.cpp
//  EE569_HW3
//
//  Created by LiuHengyue on 10/25/15.
//  Copyright © 2015 LiuHengyue. All rights reserved.
//

#include "EE569_HW3.h"

//////////////////////////////////////////////////////////
//    class constructor and member function realization
//////////////////////////////////////////////////////////

/*************************************************************************
 * surface realization
 *************************************************************************/

//constructor
//Input: face = 1 , 2, 3, etc corresponding to each face of the world cube;
//        data is one object of class imageData;
//unit is the unit length of world coordinates, default =0.01

surface::surface(int face, imageData data, float unit){
    _imageData=data;
    _unit=unit;
    int length=data._length;
    int width=data._width;
    int dimension=3;
    _coordinates=new float**[width];
    int i,j;
    //change origin to the image center
    for(i=-width/2;i<width/2;i++){//array row
        _coordinates[i+width/2]=new float*[length];
        for(j=-length/2;j<length/2;j++){//coloumn
            _coordinates[i+width/2][j+length/2]=new float[dimension];
            switch (face) {
                case 1:
                    //x axis
                    _coordinates[i+width/2][j+length/2][0]=j*unit;
                    //y axis
                    _coordinates[i+width/2][j+length/2][1]=i*unit;
                    //z axis
                    _coordinates[i+width/2][j+length/2][2]=1;
                    break;
                    
                case 2:
                    //x axis
                    _coordinates[i+width/2][j+length/2][0]=1;
                    //y axis
                    _coordinates[i+width/2][j+length/2][1]=i*unit;
                    //z axis
                    _coordinates[i+width/2][j+length/2][2]=-j*unit;
                    
                    break;
                    
                case 3:
                    //x axis
                    _coordinates[i+width/2][j+length/2][0]=-i*unit;
                    //y axis
                    _coordinates[i+width/2][j+length/2][1]=1;
                    //z axis
                    _coordinates[i+width/2][j+length/2][2]=-j*unit;
                    
                    break;

                    //
                    
                default:
                    break;
            }
        }
    }
    
}
surface::~surface(){
    
}

void surface::showInfo(){
    int i,j,dim,axis;
    for(i=0;i<_imageData._width;i++){//array row
        for(j=0;j<_imageData._length;j++){//coloumn
            cout<<"(r g b) Pixel intensity: ( ";
            for(dim=0;dim<_imageData._bytesPerPixel;dim++){
                
                cout<<int(_imageData._dataPtr[i][j][dim])<<" ";
                
            }
            cout<<")."<<endl<<"(x y z) world coordinates: ( ";

            for(axis=0;axis<3;axis++){
                cout<<_coordinates[i][j][axis]<<" ";
            }
            cout<<")."<<endl;
        }
    }
}
/*************************************************************************
 * cube realization
 *************************************************************************/

//store a surface to the cube
//Note: the order of the storation should be fixed
void cube::setSurface(surface s){
    _surface.push_back(s);
}

//project the cube to the image plane
//Input: r is the vector from orgin of world coordinates to the origin of the camera coordinates; f is the focal length.
//Output: image plane.
imageData cube::prjectCube(float r[3],float f, float density){
    if (_surface.size()<3) {
        cout<<"At least 3 surfaces are needed."<<endl;
        exit(0);
    }
    //define projected image plane, create a 500 by 500 image
    imageData projectedImage(500,500,3);
    //initialize the extrinsic matrix [R|t]
    float exMat[3][4];// 4 columns, 3 rows
    //X_c is the first row
    exMat[0][0]=-1/sqrt(2);
    exMat[0][1]=1/sqrt(2);
    exMat[0][2]=0;
    exMat[0][3]=-(r[0]*exMat[0][0]+r[1]*exMat[0][1]+r[2]*exMat[0][2]);
    //Y_c is the 2nd row
    exMat[1][0]=1/sqrt(6);
    exMat[1][1]=1/sqrt(6);
    exMat[1][2]=-2/sqrt(6);
    exMat[1][3]=-(r[0]*exMat[1][0]+r[1]*exMat[1][1]+r[2]*exMat[1][2]);
    //Z_c is the 3rd row
    exMat[2][0]=-1/sqrt(3);
    exMat[2][1]=-1/sqrt(3);
    exMat[2][2]=-1/sqrt(3);
    exMat[2][3]=-(r[0]*exMat[2][0]+r[1]*exMat[2][1]+r[2]*exMat[2][2]);
    
    //initialize the intrinsic matrix K
    //Note the image coordinates are: x=f*X_c/Z_c, y=f*Y_c/Z_c,
    //so just multiply the focal length and divided by Z_c to get the coordinates of image plane
    //find the boundary of coordinates
    float min_x=INT16_MAX,min_y=INT16_MAX;
    //iteration of each surface
    for (int i=0; i<_surface.size(); i++) {
        
        for (int X=0; X< _surface[i]._imageData._width;X++) {
            
            for (int Y=0; Y<_surface[i]._imageData._length; Y++) {
                //Z_c
                float Z_c=_surface[i]._coordinates[X][Y][0]*exMat[2][0]+_surface[i]._coordinates[X][Y][1]*exMat[2][1]+_surface[i]._coordinates[X][Y][2]*exMat[2][2]+exMat[2][3];
                //X_c
                float X_c=_surface[i]._coordinates[X][Y][0]*exMat[0][0]+_surface[i]._coordinates[X][Y][1]*exMat[0][1]+_surface[i]._coordinates[X][Y][2]*exMat[0][2]+exMat[0][3];
                //Y_c
                float Y_c=_surface[i]._coordinates[X][Y][0]*exMat[1][0]+_surface[i]._coordinates[X][Y][1]*exMat[1][1]+_surface[i]._coordinates[X][Y][2]*exMat[1][2]+exMat[1][3];
                //image coordinates
                int x=density*(f*X_c/Z_c);
                int y=density*(f*Y_c/Z_c);
                if (x<min_x) {
                    min_x=x;
                }
                if (y<min_y) {
                    min_y=y;
                }
            }
        }
    }
    //assign pixel value
    for (int i=0; i<_surface.size(); i++) {
        
        for (int X=0; X< _surface[i]._imageData._width;X++) {
            
            for (int Y=0; Y<_surface[i]._imageData._length; Y++) {
                //Z_c
                float Z_c=_surface[i]._coordinates[X][Y][0]*exMat[2][0]+_surface[i]._coordinates[X][Y][1]*exMat[2][1]+_surface[i]._coordinates[X][Y][2]*exMat[2][2]+exMat[2][3];
                //X_c
                float X_c=_surface[i]._coordinates[X][Y][0]*exMat[0][0]+_surface[i]._coordinates[X][Y][1]*exMat[0][1]+_surface[i]._coordinates[X][Y][2]*exMat[0][2]+exMat[0][3];
                //Y_c
                float Y_c=_surface[i]._coordinates[X][Y][0]*exMat[1][0]+_surface[i]._coordinates[X][Y][1]*exMat[1][1]+_surface[i]._coordinates[X][Y][2]*exMat[1][2]+exMat[1][3];
                int x=density*(f*X_c/Z_c)-min_x;
                int y=density*(f*Y_c/Z_c)-min_y;
                for (int dim=0; dim<_surface[i]._imageData._bytesPerPixel; dim++) {
                    if(x<projectedImage._width && y<projectedImage._length){
                        projectedImage._dataPtr[x][y][dim]=_surface[i]._imageData._dataPtr[X][Y][dim];
                    }
                }
            }
        }
    }
    return projectedImage;
    

}

//////////////////////////////////////////////////////////
//        other functions
//////////////////////////////////////////////////////////

/*************************************************************************
 * Swirl()
 * Function: This function reads the image data and do the swirl effect.
 * Input: ImageData; Angle; center coordinates
 * Output: swirled imageData.
 *************************************************************************/

imageData Swirl(imageData img, float angle,pair<int, int> center){
    //(u,v) is the original fractional coordinates
    float u,v;
    // (r,theta) is the polar coordinates
    float r,theta;
    float R=sqrt((img._length/2)*(img._length/2)+(img._width/2)*(img._width/2));
    //define new image
    int rows=img._length;
    int cols=img._width;
    int channels=img._bytesPerPixel;
    imageData newImg(cols,rows,channels);

    for(int dim=0;dim<channels;dim++){
        //(x,y) is the output Cartesian coordinates
        //translate the origin to center(center.x,center.y)
        for (int x=-center.first; x<cols-center.first; x++) {
            for (int y=-center.second; y<rows-center.second; y++) {
                //swirl parameters
                //polar coordinates
                r=sqrt(x*x+y*y);
                theta=atan2(float(y),float(x));
                //corresponding original fractional coordinates(u,v), note it is fraction
                //since (x,y) is the new coordinates, think the image is swirled back to the original image
                //the angle swirled is different, large radius gives large swirl effect
                u=r*cos(theta-angle*(r/R));
                v=r*sin(theta-angle*(r/R));
                //bilinear interpolation parameters
                //(u0,v0) original coordinates
                int u0=floor(u);
                int v0=floor(v);
                float a=u-u0;
                float b=v-v0;
                //only consider pixels inside of the boundary
                if(u0>=-center.first && u0<cols-center.first && v0>=-center.second && v0<rows-center.second){
                    //dealing with out of boundary for interpolation positions
                    if(u0+1+center.first<0){
                        u0=-center.first-1;
                    }
                    else if(u0+center.first+1>cols-1 ){
                        u0=cols-2-center.first ;
                    }
                    if (rows-v0-center.second <0 ) {
                        v0=rows-center.second ;
                    }
                    else if(rows-v0-center.second >rows-1){
                        v0=1-center.second;
                    }
                    //Do bilinear interpolation & translate the coordinates back
                    newImg._dataPtr[x+center.first][rows-1-y-center.second][dim]=(1-a)*(1-b)*img._dataPtr[u0+center.first][rows-1-v0-center.second][dim]+b*(1-a)*img._dataPtr[u0+center.first][rows-v0-center.second][dim]+a*(1-b)*img._dataPtr[u0+1+center.first][rows-1-v0-center.second][dim]+a*b*img._dataPtr[u0+1+center.first][rows-v0-center.second][dim];
                }
                else{
                    //if corresponding pixel is out of boundary,then set the value to zero
                    newImg._dataPtr[x+center.first][rows-1-y-center.second][dim]=0;
                }
            }
        }
    }
    
    return newImg;
}
/*************************************************************************
 * IndexMatrix()
 * Function: This function generates a N by N Bayer index matrix. Prepare
             for the threshold matrix generation.
 * Input: dimension N.
 * Output: N*N index matrix.
 *************************************************************************/
int* IndexMatrix(int index){
    
    //define Bayer index matrices
    int* In=new int[index*index];
    //using recursion method
    if(index!=2){
        //recursively generate matrices,then transform them to thresholds matrices
        //divide the matrix into 4 parts
        for (int i=0; i<index/2; i++) {
            //part with adding 1
            for (int j=0; j<index/2; j++) {
                In[i*index+j]=4*IndexMatrix(index/2)[(i%(index/2))*(index/2)+j%(index/2)]+1;
            }
            //part with adding 2
            for (int j=index/2; j<index; j++) {
                In[i*index+j]=4*IndexMatrix(index/2)[(i%(index/2))*(index/2)+j%(index/2)]+2;
            }
        }
        for (int i=index/2; i<index; i++) {
            //part with adding 3
            for (int j=0; j<index/2; j++) {
                In[i*index+j]=4*IndexMatrix(index/2)[(i%(index/2))*(index/2)+j%(index/2)]+3;
            }
            //part without adding
            for (int j=index/2; j<index; j++) {
                In[i*index+j]=4*IndexMatrix(index/2)[(i%(index/2))*(index/2)+j%(index/2)];
            }
        }
    }
    else if(index==2){
        //Bayer index matrix
        In[0]=1;
        In[1]=2;
        In[2]=3;
        In[3]=0;
    }
    else{
        cout<<"generate Matrix failed."<<endl;
        exit(0);
    }
    return In;
    
}
/*************************************************************************
 * ThresholdMatrix()
 * Function: This function generates a N by N threshold matrix. Prepare
 for the dithering process.
 * Input: dimension N.
 * Output: N*N threshold matrix.
 *************************************************************************/

float* ThresholdMatrix(int index){
    //define Bayer index matrix
    int* indexMat=IndexMatrix(index);
    //define threshold matrix
    float* thresholdMat=new float[index*index];
    //transform the index matrix into threshold matrix
    for (int i=0; i<index; i++) {
        for (int j=0; j<index; j++) {
            thresholdMat[i*index+j]=(float(indexMat[i*index+j])+0.5)/(index*index);
        }
    }
    return thresholdMat;
}
/*************************************************************************
 * BayerDither()
 * Function: This function reads the image data and do the dithering approach
             to change the image into binary image with threshold matrix.
 * Input: ImageData; threshold matrix dimension.
 * Output: dithered imageData.
 *************************************************************************/

imageData BayerDither(imageData img,int index){
    //get threshold matrix
    float* thresholdMat=ThresholdMatrix(index);
    //define dithered imageData
    imageData ditheredImg=img.createData();
    //Normalized the dynamic range to (0,1]
    //find the largest intensity
    int maxIntensity=INT_MIN;
    for (int j=0; j<img._length; j++) {//j is row
        for (int i=0; i<img._width; i++) {//i is column
            if(img._dataPtr[i][j][0]>maxIntensity){
                maxIntensity=img._dataPtr[i][j][0];
            }
        }
    }
    for (int j=0; j<img._length; j++) {//j is row
        for (int i=0; i<img._width; i++) {//i is column
            //if the normalized pixel value is larger than the threshold, set as black dot
            if(float(img._dataPtr[i][j][0])/maxIntensity>thresholdMat[(j%index)*index+(i%index)]){
                ditheredImg._dataPtr[i][j][0]=255;
            }
            //otherwise,set as white dot
            else{
                ditheredImg._dataPtr[i][j][0]=0;
            }
        }
    }
    
    return ditheredImg;
}
/*************************************************************************
 * FourLevelDither()
 * Function: This function reads the image data and do the dithering approach
 to change the image into 4-gray-level image with threshold matrix.
 * Input: ImageData; threshold matrix.
 * Output: dithered imageData.
 *************************************************************************/

imageData FourLevelDither(imageData img,int index){
    //get threshold matrix
    float* thresholdMat=ThresholdMatrix(index);
    //define dithered imageData
    imageData ditheredImg=img.createData();
    //find the largest intensity
    int maxIntensity=INT_MIN;
    for (int j=0; j<img._length; j++) {//j is row
        for (int i=0; i<img._width; i++) {//i is column
            if(img._dataPtr[i][j][0]>maxIntensity){
                maxIntensity=img._dataPtr[i][j][0];
            }
        }
    }
    //define 3 normalized thresholds for 4-level image
    float threshold[3];
    threshold[0]=float(85)/2/255;
    threshold[1]=float(170+85)/2/255;
    threshold[2]=float(255+170)/2/255;
    //Normalized the dynamic range to (0,1]
    for (int j=0; j<img._length; j++) {//j is row
        for (int i=0; i<img._width; i++) {//i is column
            //first do Bayer binary thresholding, then thresholding with 3 thresholds that classify pixels into 4 groups
            //if the normalized pixel value is larger than the threshold, set as black dot
            if(float(img._dataPtr[i][j][0])/maxIntensity>thresholdMat[(j%index)*index+(i%index)]){
                //set pixel value to 255
                if(float(img._dataPtr[i][j][0])/maxIntensity>threshold[2]){
                    ditheredImg._dataPtr[i][j][0]=255;
                }
                //set pixel value to 170
                if(float(threshold[1]<img._dataPtr[i][j][0])/maxIntensity<=threshold[2]){
                    ditheredImg._dataPtr[i][j][0]=170;
                }

                
            }
            //otherwise,set as white dot
            else{
                //set pixel value to 0
                if(float(img._dataPtr[i][j][0])/maxIntensity<threshold[0]){
                    ditheredImg._dataPtr[i][j][0]=0;
                }
                //set pixel value to 85
                if(float(threshold[0]<=img._dataPtr[i][j][0])/maxIntensity<=threshold[1]){
                    ditheredImg._dataPtr[i][j][0]=85;

                }

            }
        }
    }
    
    return ditheredImg;
}
/*************************************************************************
 * ErrorDiffusion()
 * Function: This function reads the image data and do the dithering approach
 to change the image into binary image with error diffusion methods.
 * Input: ImageData; error diffusion matrix type.
 * Output: dithered imageData.
 *************************************************************************/

imageData ErrorDiffusion(imageData img,int type){
    //define dithered imageData
    imageData ditheredImg=img.createData();
    //define error (col*row)
    float errorMat[img._width][img._length][img._bytesPerPixel];
    //Normalized the dynamic range to (0,1]
    //find the largest intensity
    int maxIntensity[img._bytesPerPixel];
    for(int dim=0;dim<img._bytesPerPixel;dim++){
        maxIntensity[dim]=INT_MIN;
        for (int i=0; i<img._width; i++) {
            for (int j=0; j<img._length; j++) {
                //initialize error to be zero
                errorMat[i][j][dim]=0;
                if(img._dataPtr[i][j][dim]>maxIntensity[dim]){
                    maxIntensity[dim]=img._dataPtr[i][j][dim];
                }
            }
        }
    }
    for(int dim=0;dim<img._bytesPerPixel;dim++){

        for (int j=0; j<img._length; j++){//j is row
              for (int i=0; i<img._width; i++){//i is column
                //adding feedback
                float F_tilde=float(img._dataPtr[i][j][dim])/maxIntensity[dim]+errorMat[i][j][dim];
                //thresholding
                //if the normalized pixel value is larger than the threshold, set value to be 1
                if(F_tilde>0.5){
                    ditheredImg._dataPtr[i][j][dim]=1;//white
                                }
                //otherwise,set value to 0
                else{
                    ditheredImg._dataPtr[i][j][dim]=0;//black
                }
                //assign quantization error
                errorMat[i][j][dim]=F_tilde-ditheredImg._dataPtr[i][j][dim];
                //cout<<errorMat[i][j][dim]<<endl;
                if(type==1){
                    //Floyd-Steinberg's error diffusion
                    //diffuse error to furture pixels, serpantine scanning
                    //left to right
                    if (j%2==0) {
                        if (i!=0 && j!=img._length-1) {
                            errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*3.0/16;
                        }
                        if (i!=img._width-1 && j!=img._length-1) {
                            errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*1.0/16;
                        }
                        if (i!=img._width-1) {
                            errorMat[i+1][j][dim]+=errorMat[i][j][dim]*7.0/16;
                        }
                        if (j!=img._length-1) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*5.0/16;
                        }
                    }
                    //right to left
                    else{
                        i=img._width-1-i;
                        if (i!=img._width-1 && j!=img._length-1) {
                            errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*3.0/16;
                        }
                        if (i!=0 && j!=img._length-1) {
                            errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*1.0/16;
                        }
                        if (i!=0) {
                            errorMat[i-1][j][dim]+=errorMat[i][j][dim]*7.0/16;
                        }
                        if (j!=img._length-1) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*5.0/16;
                        }

                    }
                }
                else if(type==2){
                    //JJN error diffusion
                    //diffuse error to furture pixels, serpantine scanning
                    //left to right
                    //right 2 pixels
                    if (j%2==0) {
                        if (i+1<img._width) {
                            errorMat[i+1][j][dim]+=errorMat[i][j][dim]*7.0/48;
                        }
                        if (i+2<img._width) {
                            errorMat[i+2][j][dim]+=errorMat[i][j][dim]*5.0/48;
                        }
                        //next row
                        if (j+1<img._length) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*7.0/48;
                            if (i-1>-1) {
                                errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*5.0/48;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+1][dim]+=errorMat[i][j][dim]*3.0/48;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*5.0/48;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+1][dim]+=errorMat[i][j][dim]*3.0/48;
                            }

                        }
                        //next next row
                        if (j+2<img._length) {
                            errorMat[i][j+2][dim]+=errorMat[i][j][dim]*5.0/48;
                            if (i-1>-1) {
                                errorMat[i-1][j+2][dim]+=errorMat[i][j][dim]*3.0/48;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+2][dim]+=errorMat[i][j][dim]*1.0/48;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+2][dim]+=errorMat[i][j][dim]*3.0/48;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+2][dim]+=errorMat[i][j][dim]*1.0/48;
                            }
                            
                        }
                        
                    }
                    //right to left
                    else{
                        i=img._width-1-i;
                        if (i-1<img._width) {
                            errorMat[i-1][j][dim]+=errorMat[i][j][dim]*7.0/48;
                        }
                        if (i-2<img._width) {
                            errorMat[i-2][j][dim]+=errorMat[i][j][dim]*5.0/48;
                        }
                        
                        if (j+1<img._length) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*7.0/48;
                            if (i-1>-1) {
                                errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*5.0/48;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+1][dim]+=errorMat[i][j][dim]*3.0/48;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*5.0/48;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+1][dim]+=errorMat[i][j][dim]*3.0/48;
                            }
                            
                        }
                        if (j+2<img._length) {
                            errorMat[i][j+2][dim]+=errorMat[i][j][dim]*5.0/48;
                            if (i-1>-1) {
                                errorMat[i-1][j+2][dim]+=errorMat[i][j][dim]*3.0/48;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+2][dim]+=errorMat[i][j][dim]*1.0/48;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+2][dim]+=errorMat[i][j][dim]*3.0/48;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+2][dim]+=errorMat[i][j][dim]*1.0/48;
                            }
                            
                        }
                        
                    }
                }
                else if(type==3){
                    //Stucki error diffusion
                    //diffuse error to furture pixels, serpantine scanning
                    //left to right
                    
                    if (j%2==0) {
                        if (i+1<img._width) {
                            errorMat[i+1][j][dim]+=errorMat[i][j][dim]*8.0/42;
                        }
                        if (i+2<img._width) {
                            errorMat[i+2][j][dim]+=errorMat[i][j][dim]*4.0/42;
                        }
                        
                        if (j+1<img._length) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*2.0/42;
                            if (i-1>-1) {
                                errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*4.0/42;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+1][dim]+=errorMat[i][j][dim]*8.0/42;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*4.0/42;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+1][dim]+=errorMat[i][j][dim]*2.0/42;
                            }
                            
                        }
                        if (j+2<img._length) {
                            errorMat[i][j+2][dim]+=errorMat[i][j][dim]*1.0/42;
                            if (i-1>-1) {
                                errorMat[i-1][j+2][dim]+=errorMat[i][j][dim]*2.0/42;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+2][dim]+=errorMat[i][j][dim]*4.0/42;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+2][dim]+=errorMat[i][j][dim]*2.0/42;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+2][dim]+=errorMat[i][j][dim]*1.0/42;
                            }
                            
                        }
                        
                    }
                    //right to left
                    else{
                        i=img._width-1-i;
                        if (i-1<img._width) {
                            errorMat[i-1][j][dim]+=errorMat[i][j][dim]*8.0/42;
                        }
                        if (i-2<img._width) {
                            errorMat[i-2][j][dim]+=errorMat[i][j][dim]*4.0/42;
                        }
                        
                        if (j+1<img._length) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*2.0/42;
                            if (i-1>-1) {
                                errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*4.0/42;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+1][dim]+=errorMat[i][j][dim]*8.0/42;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*4.0/42;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+1][dim]+=errorMat[i][j][dim]*2.0/42;
                            }
                            
                        }
                        if (j+2<img._length) {
                            errorMat[i][j+2][dim]+=errorMat[i][j][dim]*1.0/42;
                            if (i-1>-1) {
                                errorMat[i-1][j+2][dim]+=errorMat[i][j][dim]*2.0/42;
                            }
                            if (i-2>-1) {
                                errorMat[i-2][j+2][dim]+=errorMat[i][j][dim]*4.0/42;
                            }
                            if (i+1<img._width) {
                                errorMat[i+1][j+2][dim]+=errorMat[i][j][dim]*2.0/42;
                            }
                            if (i+2<img._width) {
                                errorMat[i+2][j+2][dim]+=errorMat[i][j][dim]*1.0/42;
                            }
                            
                        }
                        
                    }
                }

                else{
                    cout<<"wrong input for 'type'."<<endl;
                    exit(0);
                }
                
            }
        }
    }
    //change intensities back
    //count the number of white spaces
    int white=0;
    for (int i=0; i<img._width; i++) {//i is column
        //cout<<i<<endl;
        for (int j=0; j<img._length; j++) {//j is row
            for(int dim=0;dim<img._bytesPerPixel;dim++){
                ditheredImg._dataPtr[i][j][dim]*=255;
            }
                //cout<<int(ditheredImg._dataPtr[i][j][dim])<<endl;
            if (ditheredImg._dataPtr[i][j][0]==0 && ditheredImg._dataPtr[i][j][1] &&ditheredImg._dataPtr[i][j][2]==0) {
                white++;
                }

            
        }
    }
    cout<<white<<endl;
    return ditheredImg;
}
/*************************************************************************
 * MBVQ()
 * Function: This function reads the rgb pixel value and return its MBVQ 4
             vertexes.
 * Input: pixel value array.
 * Output: MBVQ 4 vertexes.
 *************************************************************************/
int** MBVQ(unsigned char* rgb){
    int r=rgb[0];
    int g=rgb[1];
    int b=rgb[2];
    int **mbvq=new int*[4];
    for (int i=0; i<4; i++) {
        mbvq[i]=new int[3];
    }
    if ((r+g)>255) {
        if ((g+b)>255) {
            if ((r+g+b)>510) {
                //CMYW
                //Cyan
                mbvq[0][0]=1;
                mbvq[0][1]=0;
                mbvq[0][2]=0;
                //Magenta
                mbvq[1][0]=0;
                mbvq[1][1]=1;
                mbvq[1][2]=0;
                //Yellow
                mbvq[2][0]=0;
                mbvq[2][1]=0;
                mbvq[2][2]=1;
                //White
                mbvq[3][0]=0;
                mbvq[3][1]=0;
                mbvq[3][2]=0;


                return mbvq;
            }
            else{
                //MYGC
                //Cyan
                mbvq[0][0]=1;
                mbvq[0][1]=0;
                mbvq[0][2]=0;
                //Magenta
                mbvq[1][0]=0;
                mbvq[1][1]=1;
                mbvq[1][2]=0;
                //Yellow
                mbvq[2][0]=0;
                mbvq[2][1]=0;
                mbvq[2][2]=1;
                //Green
                mbvq[3][0]=1;
                mbvq[3][1]=0;
                mbvq[3][2]=1;
                
                
                return mbvq;

            }
        }
        else{
            //RGMY
            //Red
            mbvq[0][0]=0;
            mbvq[0][1]=1;
            mbvq[0][2]=1;
            //Magenta
            mbvq[1][0]=0;
            mbvq[1][1]=1;
            mbvq[1][2]=0;
            //Yellow
            mbvq[2][0]=0;
            mbvq[2][1]=0;
            mbvq[2][2]=1;
            //Green
            mbvq[3][0]=1;
            mbvq[3][1]=0;
            mbvq[3][2]=1;
            
            
            return mbvq;

        }
    }
    else{
        if (!((g+b)>255)) {
            if (!((r+g+b)>255)) {
                //KRGB
                //Black
                mbvq[0][0]=1;
                mbvq[0][1]=1;
                mbvq[0][2]=1;
                //Red
                mbvq[1][0]=0;
                mbvq[1][1]=1;
                mbvq[1][2]=1;
                //Green
                mbvq[2][0]=1;
                mbvq[2][1]=0;
                mbvq[2][2]=1;
                //Blue
                mbvq[3][0]=1;
                mbvq[3][1]=1;
                mbvq[3][2]=0;
                
                
                return mbvq;

            }
            else{
                //RGBM
                //Magenta
                mbvq[0][0]=0;
                mbvq[0][1]=1;
                mbvq[0][2]=0;
                //Red
                mbvq[1][0]=0;
                mbvq[1][1]=1;
                mbvq[1][2]=1;
                //Green
                mbvq[2][0]=1;
                mbvq[2][1]=0;
                mbvq[2][2]=1;
                //Blue
                mbvq[3][0]=1;
                mbvq[3][1]=1;
                mbvq[3][2]=0;
                
                
                return mbvq;

            }
        }
        else{
            //CMGB
            //Cyan
            mbvq[0][0]=1;
            mbvq[0][1]=0;
            mbvq[0][2]=0;
            //Magenta
            mbvq[1][0]=0;
            mbvq[1][1]=1;
            mbvq[1][2]=0;
            //Green
            mbvq[2][0]=1;
            mbvq[2][1]=0;
            mbvq[2][2]=1;
            //Blue
            mbvq[3][0]=1;
            mbvq[3][1]=1;
            mbvq[3][2]=0;
            
            
            return mbvq;

        }
    }
    
}
/*************************************************************************
 * ColorQuantize()
 * Function: This function reads the cmy pixel value and its MBVQ, then 
             calculate the nearest distance to a certain vertex V. Then
             record the vertex values.
 * Input: pixel CMY value array; its MBVQ.
 * Output: index of the vertex.
 *************************************************************************/
int ColorQuantize(float* cmy, int** mbvq){
    //store the smallest distance
    float dis=INT16_MAX;
    //store the index of mbvq vertex
    int index=-1;
    //find the smallest distance
    for (int i=0; i<4; i++) {
        float curDis=0;
        for (int j=0; j<3; j++) {
            curDis+=(cmy[j]-mbvq[i][j])*(cmy[j]-mbvq[i][j]);
        }
        //record distance and index
        if (curDis<dis) {
            dis=curDis;
            index=i;
        }
    }
    return index;
}
/*************************************************************************
 * VectorColorHalftoning()
 * Function: This function reads the image data and do the dithering approach
             to change the color image into halftoning image via vector color
             approach and FS error diffusion.
 * Input: ImageData.
 * Output: dithered imageData.
 *************************************************************************/
imageData VectorColorHalftoning(imageData img){
    //define dithered imageData
    imageData ditheredImg=img.createData();
    //define error (col*row*dim)
    float errorMat[img._width][img._length][img._bytesPerPixel];
    //Normalized the dynamic range to (0,1]
    //find the largest intensity
    int maxIntensity[img._bytesPerPixel];
    for(int dim=0;dim<img._bytesPerPixel;dim++){
        maxIntensity[dim]=INT_MIN;
        for (int i=0; i<img._width; i++) {
            for (int j=0; j<img._length; j++) {
                //initialize error to be zero
                errorMat[i][j][dim]=0;
                if(img._dataPtr[i][j][dim]>maxIntensity[dim]){
                    maxIntensity[dim]=img._dataPtr[i][j][dim];
                }
            }
        }
    }
        for (int j=0; j<img._length; j++) {//j is row
            for (int i=0; i<img._width; i++) {//i is column
                //find the MBVQ four vertexes
                int** mbvq=MBVQ(img._dataPtr[i][j]);
                //normalize pixel values
                float CMY[3];
                for (int dim=0; dim<3; dim++) {
                    //change rgb to cmy
                    CMY[dim]=1-float(img._dataPtr[i][j][dim])/maxIntensity[dim];
                    //cout<<CMY[dim]<<endl;
                }
                //adding feedback
                for (int dim=0; dim<3; dim++) {
                    CMY[dim]+=errorMat[i][j][dim];
                    //cout<<CMY[dim]<<endl;

                }
                //find which color this pixel should be quantized to
                int aim=ColorQuantize(CMY, mbvq);

                //quantize the color
                for (int dim=0; dim<3; dim++) {
                    ditheredImg._dataPtr[i][j][dim]=mbvq[aim][dim];
                    //assign error
                    errorMat[i][j][dim]=CMY[dim]-mbvq[aim][dim];
                    
                    //cout<<errorMat[i][j][dim]<<endl;
                    //Floyd-Steinberg's error diffusion
                    //diffuse error to furture pixels, serpantine scanning
                    //left to right
                    if (j%2==0) {
                        if (i!=0 && j!=img._length-1) {
                            errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*3.0/16;
                        }
                        if (i!=img._width-1 && j!=img._length-1) {
                            errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*1.0/16;
                        }
                        if (i!=img._width-1) {
                            errorMat[i+1][j][dim]+=errorMat[i][j][dim]*7.0/16;
                        }
                        if (j!=img._length-1) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*5.0/16;
                        }
                    }
                    //right to left
                    else{
                        i=img._width-1-i;
                        if (i!=img._width-1 && j!=img._length-1) {
                            errorMat[i+1][j+1][dim]+=errorMat[i][j][dim]*3.0/16;
                        }
                        if (i!=0 && j!=img._length-1) {
                            errorMat[i-1][j+1][dim]+=errorMat[i][j][dim]*1.0/16;
                        }
                        if (i!=0) {
                            errorMat[i-1][j][dim]+=errorMat[i][j][dim]*7.0/16;
                        }
                        if (j!=img._length-1) {
                            errorMat[i][j+1][dim]+=errorMat[i][j][dim]*5.0/16;
                        }
                        
                    }

                }
         
            }
        }
    //count the number of white spaces
    int white=0;
    //change intensities back
    for (int i=0; i<img._width; i++) {//i is column
        //cout<<i<<endl;
        for (int j=0; j<img._length; j++) {//j is row
            for(int dim=0;dim<img._bytesPerPixel;dim++){
            ditheredImg._dataPtr[i][j][dim]=255-ditheredImg._dataPtr[i][j][dim]*255;
            }
            if (ditheredImg._dataPtr[i][j][0]==0 && ditheredImg._dataPtr[i][j][1] &&ditheredImg._dataPtr[i][j][2]==0) {
                white++;
            }
        }
    }
    cout<<white<<endl;
    return ditheredImg;
    
}
//define shrinking filter
    int SNUM=58;
    int SHRINKING[58][8]={
        //S1
        0,1,0,0,0,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,0,0,1,
        //S2
        1,0,0,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,0,0,1,0,
        //S3
        1,1,0,0,0,0,0,0,
        0,1,1,0,0,0,0,0,
        0,0,1,1,0,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,1,1,0,0,
        0,0,0,0,0,1,1,0,
        0,0,0,0,0,0,1,1,
        1,0,0,0,0,0,0,1,
        //TK4
        //1,0,1,0,0,0,0,0,
        //0,0,1,0,1,0,0,0,
        //0,0,0,0,1,0,1,0,
        //1,0,0,0,0,0,1,0,
        //STK4
        1,1,0,0,0,0,0,1,
        0,1,1,1,0,0,0,0,
        0,0,0,1,1,1,0,0,
        0,0,0,0,0,1,1,1,
        //ST5
        1,0,1,1,0,0,0,0,
        1,0,1,0,0,0,0,1,
        0,1,1,0,1,0,0,0,
        1,1,0,0,0,0,1,0,
        //ST5
        1,1,1,0,0,0,0,0,
        0,0,1,1,1,0,0,0,
        0,0,0,0,1,1,1,0,
        1,0,0,0,0,0,1,1,
        //ST6
        1,0,1,1,0,0,0,1,
        0,1,1,0,1,1,0,0,
        //STK6
        1,1,1,1,0,0,0,0,
        1,1,1,0,0,0,0,1,
        0,1,1,1,1,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,0,1,1,1,1,0,
        0,0,0,0,1,1,1,1,
        1,0,0,0,0,1,1,1,
        1,1,0,0,0,0,1,1,
        //STK7
        1,1,1,1,0,0,0,1,
        0,1,1,1,1,1,0,0,
        0,0,0,1,1,1,1,1,
        1,1,0,0,0,1,1,1,
        //STK8
        1,1,1,0,0,0,1,1,
        1,1,1,1,1,0,0,0,
        0,0,1,1,1,1,1,0,
        1,0,0,0,1,1,1,1,
        //STK9
        1,1,1,1,0,0,1,1,
        1,1,1,0,0,1,1,1,
        1,1,1,1,1,1,0,0,
        1,1,1,1,1,0,0,1,
        0,1,1,1,1,1,1,0,
        0,0,1,1,1,1,1,1,
        1,0,0,1,1,1,1,1,
        1,1,0,0,1,1,1,1,
        //STK10
        1,1,1,1,0,1,1,1,
        1,1,1,1,1,1,0,1,
        0,1,1,1,1,1,1,1,
        1,1,0,1,1,1,1,1,
        //K11
        //1,1,1,1,1,0,1,1,
        //1,1,1,1,1,1,1,0,
        //1,0,1,1,1,1,1,1,
        //1,1,1,0,1,1,1,1,
    };
//define thining filter
    int TNUM=46;
    int THINING[46][8]={
        //S1
        //0,1,0,0,0,0,0,0,
        //0,0,0,1,0,0,0,0,
        //0,0,0,0,0,1,0,0,
        //0,0,0,0,0,0,0,1,
        //S2
        //1,0,0,0,0,0,0,0,
        //0,0,1,0,0,0,0,0,
        //0,0,0,0,1,0,0,0,
        //0,0,0,0,0,0,1,0,
        //S3
        //1,1,0,0,0,0,0,0,
        //0,1,1,0,0,0,0,0,
        //0,0,1,1,0,0,0,0,
        //0,0,0,1,1,0,0,0,
        //0,0,0,0,1,1,0,0,
        //0,0,0,0,0,1,1,0,
        //0,0,0,0,0,0,1,1,
        //1,0,0,0,0,0,0,1,
        //TK4
        1,0,1,0,0,0,0,0,
        0,0,1,0,1,0,0,0,
        0,0,0,0,1,0,1,0,
        1,0,0,0,0,0,1,0,
        //STK4
        1,1,0,0,0,0,0,1,
        0,1,1,1,0,0,0,0,
        0,0,0,1,1,1,0,0,
        0,0,0,0,0,1,1,1,
        //ST5
        1,0,1,1,0,0,0,0,
        1,0,1,0,0,0,0,1,
        0,1,1,0,1,0,0,0,
        1,1,0,0,0,0,1,0,
        //ST5
        1,1,1,0,0,0,0,0,
        0,0,1,1,1,0,0,0,
        0,0,0,0,1,1,1,0,
        1,0,0,0,0,0,1,1,
        //ST6
        1,0,1,1,0,0,0,1,
        0,1,1,0,1,1,0,0,
        //STK6
        1,1,1,1,0,0,0,0,
        1,1,1,0,0,0,0,1,
        0,1,1,1,1,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,0,1,1,1,1,0,
        0,0,0,0,1,1,1,1,
        1,0,0,0,0,1,1,1,
        1,1,0,0,0,0,1,1,
        //STK7
        1,1,1,1,0,0,0,1,
        0,1,1,1,1,1,0,0,
        0,0,0,1,1,1,1,1,
        1,1,0,0,0,1,1,1,
        //STK8
        1,1,1,0,0,0,1,1,
        1,1,1,1,1,0,0,0,
        0,0,1,1,1,1,1,0,
        1,0,0,0,1,1,1,1,
        //STK9
        1,1,1,1,0,0,1,1,
        1,1,1,0,0,1,1,1,
        1,1,1,1,1,1,0,0,
        1,1,1,1,1,0,0,1,
        0,1,1,1,1,1,1,0,
        0,0,1,1,1,1,1,1,
        1,0,0,1,1,1,1,1,
        1,1,0,0,1,1,1,1,
        //STK10
        1,1,1,1,0,1,1,1,
        1,1,1,1,1,1,0,1,
        0,1,1,1,1,1,1,1,
        1,1,0,1,1,1,1,1,
        //K11
        //1,1,1,1,1,0,1,1,
        //1,1,1,1,1,1,1,0,
        //1,0,1,1,1,1,1,1,
        //1,1,1,0,1,1,1,1,
    };
//define skeletonizing filter
int KNUM=40;
int SKELETONIZING[40][8]={
    //S1
    //0,1,0,0,0,0,0,0,
    //0,0,0,1,0,0,0,0,
    //0,0,0,0,0,1,0,0,
    //0,0,0,0,0,0,0,1,
    //S2
    //1,0,0,0,0,0,0,0,
    //0,0,1,0,0,0,0,0,
    //0,0,0,0,1,0,0,0,
    //0,0,0,0,0,0,1,0,
    //S3
    //1,1,0,0,0,0,0,0,
    //0,1,1,0,0,0,0,0,
    //0,0,1,1,0,0,0,0,
    //0,0,0,1,1,0,0,0,
    //0,0,0,0,1,1,0,0,
    //0,0,0,0,0,1,1,0,
    //0,0,0,0,0,0,1,1,
    //1,0,0,0,0,0,0,1,
    //TK4
    1,0,1,0,0,0,0,0,
    0,0,1,0,1,0,0,0,
    0,0,0,0,1,0,1,0,
    1,0,0,0,0,0,1,0,
    //STK4
    1,1,0,0,0,0,0,1,
    0,1,1,1,0,0,0,0,
    0,0,0,1,1,1,0,0,
    0,0,0,0,0,1,1,1,
    //ST5
    //1,0,1,1,0,0,0,0,
    //1,0,1,0,0,0,0,1,
    //0,1,1,0,1,0,0,0,
    //1,1,0,0,0,0,1,0,
    //ST5
    //1,1,1,0,0,0,0,0,
    //0,0,1,1,1,0,0,0,
    //0,0,0,0,1,1,1,0,
    //1,0,0,0,0,0,1,1,
    //ST6
    //1,0,1,1,0,0,0,1,
    //0,1,1,0,1,1,0,0,
    //STK6
    1,1,1,1,0,0,0,0,
    1,1,1,0,0,0,0,1,
    0,1,1,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,1,1,0,
    0,0,0,0,1,1,1,1,
    1,0,0,0,0,1,1,1,
    1,1,0,0,0,0,1,1,
    //STK7
    1,1,1,1,0,0,0,1,
    0,1,1,1,1,1,0,0,
    0,0,0,1,1,1,1,1,
    1,1,0,0,0,1,1,1,
    //STK8
    1,1,1,0,0,0,1,1,
    1,1,1,1,1,0,0,0,
    0,0,1,1,1,1,1,0,
    1,0,0,0,1,1,1,1,
    //STK9
    1,1,1,1,0,0,1,1,
    1,1,1,0,0,1,1,1,
    1,1,1,1,1,1,0,0,
    1,1,1,1,1,0,0,1,
    0,1,1,1,1,1,1,0,
    0,0,1,1,1,1,1,1,
    1,0,0,1,1,1,1,1,
    1,1,0,0,1,1,1,1,
    //STK10
    1,1,1,1,0,1,1,1,
    1,1,1,1,1,1,0,1,
    0,1,1,1,1,1,1,1,
    1,1,0,1,1,1,1,1,
    //K11
    1,1,1,1,1,0,1,1,
    1,1,1,1,1,1,1,0,
    1,0,1,1,1,1,1,1,
    1,1,1,0,1,1,1,1,
};

//define second h/m filter
//-1 denotes where do not care
    int STNUMII=70;
    int STII[70][8]={
        //Spur 2
        0,1,0,0,0,0,0,0,
        0,0,0,1,0,0,0,0,
        //single 4-connection 2
        0,0,0,0,0,0,1,0,
        1,0,0,0,0,0,0,0,
        //L cluster 8
        1,1,0,0,0,0,0,0,
        0,1,1,0,0,0,0,0,
        0,0,1,1,0,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,1,1,0,0,
        0,0,0,0,0,1,1,0,
        0,0,0,0,0,0,1,1,
        1,0,0,0,0,0,0,1,
        //4-connected offset 4
        0,1,1,0,1,0,0,0,
        1,0,1,1,0,0,0,0,
        1,0,1,0,0,0,0,1,
        1,1,0,0,0,0,1,0,
        //spur corner cluster 12
        1,1,1,0,0,1,0,0,
        0,1,1,0,0,1,0,0,
        1,1,0,0,0,1,0,0,
        
        0,0,1,1,1,0,0,1,
        0,0,0,1,1,0,0,1,
        0,0,1,1,0,0,0,1,
        
        0,1,0,0,1,1,1,0,
        0,1,0,0,0,1,1,0,
        0,1,0,0,1,1,0,0,
        
        1,0,0,1,0,0,1,1,
        0,0,0,1,0,0,1,1,
        1,0,0,1,0,0,0,1,
        //corner cluster 1
        -1,-1,1,1,1,-1,-1,-1,
        //1,1,1,1,1,1,1,1,
        //Tee branch 8
        1,0,1,-1,1,-1,0,0,
        //1,0,1,1,1,1,0,0,
        1,-1,1,0,1,0,0,-1,
        1,-1,0,0,1,0,1,-1,
        1,0,0,-1,1,-1,1,0,
        0,0,1,-1,1,-1,1,0,
        //0,1,1,1,1,0,1,0,
        0,0,1,0,1,-1,1,-1,
        //1,0,1,0,0,0,1,0,
        1,0,1,0,0,-1,1,-1,
        1,-1,1,-1,0,0,1,0,
        //1,1,1,1,0,0,1,0,
        //Vee branch 28
        -1,1,-1,1,-1,1,1,1,
        -1,1,-1,1,-1,0,1,1,
        -1,1,-1,1,-1,1,0,1,
        -1,1,-1,1,-1,1,1,0,
        -1,1,-1,1,-1,0,0,1,
        -1,1,-1,1,-1,1,0,0,
        -1,1,-1,1,-1,0,1,0,
        
        1,1,-1,1,-1,1,-1,1,
        1,1,-1,1,-1,1,-1,0,
        0,1,-1,1,-1,1,-1,1,
        1,0,-1,1,-1,1,-1,1,
        0,0,-1,1,-1,1,-1,1,
        1,0,-1,1,-1,1,-1,0,
        0,1,-1,1,-1,1,-1,0,
        
        -1,1,1,1,-1,1,-1,1,
        -1,0,1,1,-1,1,-1,1,
        -1,1,0,1,-1,1,-1,1,
        -1,1,1,0,-1,1,-1,1,
        -1,1,0,0,-1,1,-1,1,
        -1,0,1,0,-1,1,-1,1,
        -1,0,0,1,-1,1,-1,1,
        
        -1,1,-1,1,1,1,-1,1,
        -1,1,-1,0,1,1,-1,1,
        -1,1,-1,1,0,1,-1,1,
        -1,1,-1,1,1,0,-1,1,
        -1,1,-1,1,0,0,-1,1,
        -1,1,-1,0,1,0,-1,1,
        -1,1,-1,0,0,1,-1,1,
        //diagonal branch 4
        
        //1,0,1,0,0,1,0,0,
        1,0,1,-1,0,1,0,-1,
        //0,0,1,0,1,0,0,1,
        0,-1,1,0,1,-1,0,1,
        //0,1,0,0,1,0,1,0,
        0,1,0,-1,1,0,1,-1,
        //1,0,0,1,0,0,1,0,
        1,-1,0,1,0,-1,1,0,
        
    };
//define second h/m filter
//-1 denotes where do not care
int KNUMII=50;
int KII[50][8]={
    //Spur 4
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,1,0,0,
    0,1,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,
    //single 4-connection 4
    1,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,0,0,0,0,1,0,
    1,0,0,0,0,0,0,0,
    //L cluster 4
    1,0,1,0,0,0,0,0,
    0,0,1,0,1,0,0,0,
    0,0,0,0,1,0,1,0,
    1,0,0,0,0,0,1,0,
    //corner cluster 2
    -1,-1,1,1,1,-1,-1,-1,
    1,-1,-1,-1,-1,-1,1,1,
    //Tee branch 4
    1,-1,1,-1,1,-1,-1,-1,
    -1,-1,1,-1,1,-1,1,-1,
    1,-1,-1,-1,1,-1,1,-1,
    1,-1,1,-1,-1,-1,1,-1,
    //Vee branch 28
    -1,1,-1,1,-1,1,1,1,
    -1,1,-1,1,-1,0,1,1,
    -1,1,-1,1,-1,1,0,1,
    -1,1,-1,1,-1,1,1,0,
    -1,1,-1,1,-1,0,0,1,
    -1,1,-1,1,-1,1,0,0,
    -1,1,-1,1,-1,0,1,0,
    
    1,1,-1,1,-1,1,-1,1,
    1,1,-1,1,-1,1,-1,0,
    0,1,-1,1,-1,1,-1,1,
    1,0,-1,1,-1,1,-1,1,
    0,0,-1,1,-1,1,-1,1,
    1,0,-1,1,-1,1,-1,0,
    0,1,-1,1,-1,1,-1,0,
    
    -1,1,1,1,-1,1,-1,1,
    -1,0,1,1,-1,1,-1,1,
    -1,1,0,1,-1,1,-1,1,
    -1,1,1,0,-1,1,-1,1,
    -1,1,0,0,-1,1,-1,1,
    -1,0,1,0,-1,1,-1,1,
    -1,0,0,1,-1,1,-1,1,
    
    -1,1,-1,1,1,1,-1,1,
    -1,1,-1,0,1,1,-1,1,
    -1,1,-1,1,0,1,-1,1,
    -1,1,-1,1,1,0,-1,1,
    -1,1,-1,1,0,0,-1,1,
    -1,1,-1,0,1,0,-1,1,
    -1,1,-1,0,0,1,-1,1,
    //diagonal branch 4
    
    //1,0,1,0,0,1,0,0,
    1,0,1,-1,0,1,0,-1,
    //0,0,1,0,1,0,0,1,
    0,-1,1,0,1,-1,0,1,
    //0,1,0,0,1,0,1,0,
    0,1,0,-1,1,0,1,-1,
    //1,0,0,1,0,0,1,0,
    1,-1,0,1,0,-1,1,0,
    
};

/*************************************************************************
 * HitOrMiss()
 * Function: This function reads the normalized image data and apply
             the hit or miss filters.
 * Input: ImageData; morphological filters I and II.
 * Output: morphological imageData.
 *************************************************************************/
imageData HitOrMiss(imageData img, int numI, int filterI[][8], int numII, int filterII[][8]){
    //middle array
    imageData mArray=img.createData();
    //output image
    imageData outputG=img.createData();
    int xShift[8]={1,1,0,-1,-1,-1,0,1};
    int yShift[8]={0,1,1,1,0,-1,-1,-1};

    
    for (int i=0; i<img._width; i++) {

        for (int j=0; j<img._length; j++) {
            //check if the pixel is interested
            if(img._dataPtr[i][j][0]==1){
                //boolean for whether the pixel should be a candidate for each filter,default yes
                bool candidate[numI];
                //hit or miss of this pixel
                bool hit=0;
                //apply each filter within window
                for (int f=0; f<numI; f++) {
                    candidate[f]=1;
                    for (int w=0; w<8; w++) {
                        
                        if ((0<=(i+xShift[w]) && (i+xShift[w])<img._width && (0<=(j+yShift[w]) && (j+yShift[w])<img._length))) {
                            //cout<<(i+xShift[w])<<" "<<(j+yShift[w])<<endl;
                            if (img._dataPtr[i+xShift[w]][j+yShift[w]][0]!=filterI[f][w]) {
                                candidate[f]=0;
                                break;
                            }
                        }
                        //set out of boundary to zero
                        else{
                            
                            if (0!=filterI[f][w]) {
                                candidate[f]=0;
                                break;
                            }
                        }
                    }
                    //hit or miss of this filter
                    hit=hit||candidate[f];
                    
                    
                }
                //if the pixel hits any filter, then it is a candidate
                if (hit==true) {
                    mArray._dataPtr[i][j][0]=1;
                }
                else{
                    mArray._dataPtr[i][j][0]=0;
                    
                }
            }
        }
    }
    //ShowImg("marray", mArray);
    //second filter
    //record number of removals
    int removalCount=0;
    for (int i=0; i<mArray._width; i++) {
        for (int j=0; j<mArray._length; j++) {
            //cout<<int(mArray._dataPtr[i][j][0])<<" ";
            //check if the pixel is candidate
            if(mArray._dataPtr[i][j][0]==1){
                //boolean for whether the pixel should be a candidate for each filter,default yes
                bool candidate[numII];
                //hit or miss of this pixel
                bool hit=0;
                //apply each filter within window
                for (int f=0; f<numII; f++) {
                    candidate[f]=1;
                    for (int w=0; w<8; w++) {
                        if ((0<=(i+xShift[w]) && (i+xShift[w])<img._width && (0<=(j+yShift[w]) && (j+yShift[w])<img._length))) {
                            if (mArray._dataPtr[i+xShift[w]][j+yShift[w]][0]!=filterII[f][w] && filterII[f][w]!=-1) {
                                candidate[f]=0;
                            }
                        }
                        //set out of boundary to zero
                        else{
                            if (0!=filterII[f][w] && filterII[f][w]!=-1) {
                                candidate[f]=0;
                            }
                        }
                    }
                    //hit or miss of this filter
                    hit=hit||candidate[f];
                    
                }
                //if the pixel hits any filter, then it is a candidate
                if (hit==true) {
                    outputG._dataPtr[i][j][0]=img._dataPtr[i][j][0];
                    
                }
                else{
                    outputG._dataPtr[i][j][0]=0;
                    removalCount++;
                }
            }
            else{
                outputG._dataPtr[i][j][0]=img._dataPtr[i][j][0];
            }
        }
        //cout<<endl;
    }
    
    
    if (removalCount!=0) {
        //cout<<removalCount<<endl;
        return HitOrMiss(outputG,numI,filterI,numII,filterII);
    }
    else{
        //counting isolated dots
        int singleDots=0;
        for (int i=0; i<outputG._width; i++) {
            for (int j=0; j<outputG._length; j++) {
                if (outputG._dataPtr[i][j][0]==1) {
                    int neighbors=0;
                    for (int w=0; w<8; w++) {
                        if ((0<=(i+xShift[w]) && (i+xShift[w])<img._width && (0<=(j+yShift[w]) && (j+yShift[w])<img._length))) {
                            if (outputG._dataPtr[i+xShift[w]][j+yShift[w]][0]!=0) {
                                neighbors++;
                            }
                        }
                    }
                    if (neighbors==0) {
                        singleDots++;
                    }
                }
                //change the range to 0-255
                outputG._dataPtr[i][j][0]=outputG._dataPtr[i][j][0]*255;
                
            }
        }
        //show isolated dots
        //cout<<"Isolated dots number is: "<<singleDots<<endl;
        return outputG;
    }
}

    

/*************************************************************************
 * Morphing()
 * Function: This function reads the image data and apply the hit or miss
             filters to achieve shrinking/ thining / skeletonizing.
 * Input: ImageData; type for shrinking, thinning or skeletonizing; 
          int inverse: inverse the color of image (default 0).
 * Output: morphological imageData.
 *************************************************************************/
imageData Morphing(imageData img, int type, int inverse){
    //copy the image data for processing
    imageData copy=img.copy();
    //output image
    imageData outputG;
    //normalize values to 0 and 1
    for (int i=0; i<copy._width; i++) {
        for (int j=0; j<copy._length; j++) {
            if (inverse==1) {
                copy._dataPtr[i][j][0]=1-copy._dataPtr[i][j][0]/255;
            }
            else{
                copy._dataPtr[i][j][0]=copy._dataPtr[i][j][0]/255;
            }
        }
    }
    switch (type) {
        //shrinking
        case 1:
        {
            //show current filter information
            cout<<"Conducting shrinking hit or miss filters..."<<endl;
            //shrinking
            outputG=HitOrMiss(copy, SNUM, SHRINKING, STNUMII, STII);
        
            break;
        }
        //thinning
        case 2:
        {
            //show current filter information
            //cout<<"Conducting thinning hit or miss filters..."<<endl;
            //thinning
            outputG=HitOrMiss(copy, TNUM, THINING, STNUMII, STII);
            break;
        }
        //skeletonizing
        case 3:
        {
            //show current filter information
            cout<<"Conducting skeletonizing hit or miss filters..."<<endl;
            //thinning
            outputG=HitOrMiss(copy, KNUM, SKELETONIZING, KNUMII, KII);
            break;
        }

        default:
            return outputG;
            break;
    }

    
    return outputG;
}
/*************************************************************************
 * HoleFilling()
 * Function: This function reads the image data and apply hole-filling
             filter to the image.
 * Input: ImageData; shrinked holes locations
 * Output: hole-filled imageData.
 *************************************************************************/

imageData HoleFilling(imageData img,bool inverse){
    imageData copied=img.copy();
    //imageData outputG=img.copy();
    //find initial fill position//
    //imageData loc=Morphing(copied,1,1);
    int xShift[4]={1,0,-1,0};
    int yShift[4]={0,1,0,-1};
    int xWindow[8]={1,1,0,0,-1,-1,0,1};
    int yWindow[8]={0,1,1,1,0,-1,-1,-1};

    int fillNum=0;
    for (int i=2; i<copied._width/2; i++) {
        for (int j=2; j<copied._length/2; j++) {
            if (copied._dataPtr[i][j][0]==255*inverse && copied._dataPtr[i][j-2][0]==255*(1-inverse) && copied._dataPtr[i-2][j][0]==255*(1-inverse)) {
                copied._dataPtr[i][j][0]=255*(1-inverse);
                fillNum++;
            }
            
        }
        for (int j=copied._length/2; j<copied._length-2; j++) {
            if (copied._dataPtr[i][j][0]==255*inverse && copied._dataPtr[i][j+2][0]==255*(1-inverse) && copied._dataPtr[i-2][j][0]==255*(1-inverse)) {
                copied._dataPtr[i][j][0]=255*(1-inverse);
                fillNum++;
            }
            
        }

    }for (int i=copied._width/2; i<copied._width-2; i++) {
        for (int j=2; j<copied._length/2; j++) {
            if (copied._dataPtr[i][j][0]==255*inverse && copied._dataPtr[i][j-2][0]==255*(1-inverse) && copied._dataPtr[i+2][j][0]==255*(1-inverse)) {
                copied._dataPtr[i][j][0]=255*(1-inverse);
                fillNum++;
            }
            
        }
        for (int j=copied._length/2; j<copied._length-2; j++) {
            if (copied._dataPtr[i][j][0]==255*inverse && copied._dataPtr[i][j+2][0]==255*(1-inverse) && copied._dataPtr[i+2][j][0]==255*(1-inverse)) {
                copied._dataPtr[i][j][0]=255*(1-inverse);
                fillNum++;
            }
            
        }
        
    }
    if (fillNum!=0) {
        return HoleFilling(copied,inverse);
    }
    else{
        return copied;
    }
    /*
     {
     
     int neighbors=0;
     for (int w=0; w<9; w++) {
     if ((0<=(i+xWindow[w]) && (i+xWindow[w])<loc._width && (0<=(j+yWindow[w]) && (j+yWindow[w])<loc._length))) {
     neighbors+=copied._dataPtr[i+xWindow[w]][j+yWindow[w]][0]/255;
     }
     }
     if(neighbors!=0){
     loc._dataPtr[i][j][0]=0;
     for (int w=0; w<4; w++) {
     if ((0<=(i+xShift[w]) && (i+xShift[w])<loc._width && (0<=(j+yShift[w]) && (j+yShift[w])<loc._length))) {
     if (copied._dataPtr[i+xShift[w]][j+yShift[w]][0]==255) {
     //cout<<"yes"<<endl;
     copied._dataPtr[i][j][0]=255;
     fillNum++;
     break;
     
     }
     }
     }
     }
     else{
     //loc._dataPtr[i][j][0]=0;
     copied._dataPtr[i][j][0]=0;
     
     }
     
     }
     }
     if (fillNum!=0) {
     cout<<fillNum<<endl;
     return HoleFilling(copied,loc);
     //return loc;
     }
     else{
     cout<<fillNum<<endl;
     return copied;
     }
     
     */
}

imageData ConectedLabeling(imageData img){
    imageData copied=img.copy();
    //construct a labeled image
    imageData labeledImg(img._width, img._length, 3);
    //store linked labels
    vector<vector<int>> linked;
    //shift steps
    int xWindow[4]={1,0,-1,0};
    int yWindow[4]={0,1,0,-0};
    //store the labels
    int labels[copied._width][copied._length];
    int currentLabel=0;
    for (int i=0; i<copied._width; i++) {
        for (int j=0; j<copied._length; j++) {
            //initialize labels
            labels[i][j]=0;
            //find interior point
            //cout<<int(copied._dataPtr[i][j][0])<<endl;
            if (copied._dataPtr[i][j][0]==0) {
                labels[i][j]=-1;
            }
            else {
                //check if assgined label
                if(labels[i][j]==0){
                //4-connected
                    if ((i-1)>-1 && (i-1)<copied._width && labels[i-1][j]!=-1) {
                        labels[i][j]=labels[i-1][j];
                    
                    }
                    else if ((j-1)>-1 && (j-1)<copied._length && labels[i][j-1]!=-1) {
                        labels[i][j]=labels[i][j-1];
                        
                    }
                    else{
                        currentLabel++;
                        labels[i][j]=currentLabel;
                        vector<int> curLabel;
                        curLabel.push_back(currentLabel);
                        for(int w=0;w<4;w++){
                                if(labels[i+xWindow[w]][j+xWindow[w]]>-1 && labels[i+xWindow[w]][j+xWindow[w]]<labels[i][j]){
                                    labels[i][j]=labels[i+xWindow[w]][j+xWindow[w]];
                                //changed++;
                                
                            }
                        }

                        linked.push_back(curLabel);
                    }

                }
                
            }
            labeledImg._dataPtr[i][j][0]=(labels[i][j]+1)*5;
            labeledImg._dataPtr[i][j][1]=(labels[i][j]+1)*10;
            labeledImg._dataPtr[i][j][2]=(labels[i][j]+1)*20;

            //cout<<labels[i][j]<<"  ";
        }
        //cout<<endl;
    }
    //int linked[currentLabel][currentLabel];
    int changed=0;
    for (int i=0; i<copied._width; i++) {
        for (int j=0; j<copied._length; j++) {
            if (labels[i][j]!=-1) {
                for(int w=0;w<4;w++){
                if ((0<=(i+xWindow[w]) && (i+xWindow[w])<copied._width && (0<=(j+yWindow[w]) && (j+yWindow[w])<copied._length))) {
                    if(labels[i+xWindow[w]][j+xWindow[w]]>-1 && labels[i+xWindow[w]][j+xWindow[w]]<labels[i][j]){
                        labels[i][j]=labels[i+xWindow[w]][j+xWindow[w]];
                        changed++;
                    }
                }
            }
            
            labeledImg._dataPtr[i][j][0]=(labels[i][j]+1)*5;
            labeledImg._dataPtr[i][j][1]=(labels[i][j]+1)*10;
            labeledImg._dataPtr[i][j][2]=(labels[i][j]+1)*20;
            
            }
        }
        //cout<<endl;
    }

    return labeledImg;
    
}
int main(int argc, const char * argv[]) {
    if(argc!=2){
        cout<<"please enter problem number: 1, 2 or 3."<<endl;
    }
    //////////////////////////////////////////////////////////
    // Problem 1 (a)
    //////////////////////////////////////////////////////////
    switch(atoi(argv[1])){
        case 1:
        {
            cout<<"which sub problem, enter a or b:"<<endl;
            char q;
            cin>>q;
            if(q=='a'){
                const char* KATE="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P1/kate.raw";
                pair<int, int> b(256,256);
                imageData kate(KATE, 512, 512, 3);
                imageData swirlKate=Swirl(kate, 3*M_PI/4, b);
                ShowImg("kate.raw",kate);
            }
            if(q=='b'){
                //////////////////////////////////////////////////////////
                //Problem 1 (b)
                //////////////////////////////////////////////////////////
                //surf1.showInfo();
                const char* BABY="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P2/baby.raw";
                const char* CAT="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P2/baby_cat.raw";
                const char* DOG="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P2/baby_dog.raw";
                cube worldCube;
                surface baby(1,imageData(BABY,200,200,3));
                worldCube.setSurface(baby);
                //display the intensity value and coordinates of a surface
                baby.showInfo();
                surface cat(2,imageData(CAT,200,200,3));
                worldCube.setSurface(cat);
                surface dog(3,imageData(DOG,200,200,3));
                worldCube.setSurface(dog);
                float r[]={5,5,5};
                imageData projectedImg= worldCube.prjectCube(r, sqrt(3),700);
                ShowImg("projected cube with pixel density 700",projectedImg);
            }
            break;
        }
        case 2:
        {
            
            cout<<"which sub problem, enter a, b, c or d:"<<endl;
            char q;
            cin>>q;
            //define 3 error diffusion methods parameter
            int FS=1;
            int JJN=2;
            int STUCKI=3;
            //input images
            const char* MANDRILL="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P2/mandrill.raw";
            imageData mandrill(MANDRILL,512,512,1);
            const char* SAILBOAT="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P2/Sailboat.raw";
            imageData sailBoat(SAILBOAT,512,512,3);
            //////////////////////////////////////////////////////////
            //Problem 2 (a)
            //////////////////////////////////////////////////////////
            
            if(q=='a'){
                
                ShowImg("mandrill",mandrill);
                //4*4 Bayer dithering
                ShowImg("I-4 dithered 'mandrill.raw'",BayerDither(mandrill, 4));
                //8*8 Bayer dithering
                ShowImg("I-8 dithered 'mandrill.raw'",BayerDither(mandrill, 8));
                //display with 4 levels
                ShowImg("dithered 'mandrill.raw' with 4 gray levels ",FourLevelDither(mandrill, 4));
            }
            
            //////////////////////////////////////////////////////////
            //Problem 2 (b)
            //////////////////////////////////////////////////////////
            
            if(q=='b'){
                
                //error diffusion methods
                ShowImg("F-S error diffusion 'mandrill.raw'",ErrorDiffusion(mandrill, FS));
                ShowImg("JJN error diffusion 'mandrill.raw'",ErrorDiffusion(mandrill, JJN));
                ShowImg("Stucki error diffusion 'mandrill.raw'",ErrorDiffusion(mandrill, STUCKI));
            }
            
            //////////////////////////////////////////////////////////
            //Problem 2 (c)
            //////////////////////////////////////////////////////////
            
            if(q=='c'){
                
                ShowImg("Sailboat.raw", sailBoat);
                ShowImg("F-S error diffusion 'Sailboat.raw'",ErrorDiffusion(sailBoat, FS));
                ShowImg("Color vector F-S error diffusion 'Sailboat.raw'",VectorColorHalftoning(sailBoat));
            }
            
            //////////////////////////////////////////////////////////
            //Problem 2 (d)
            //////////////////////////////////////////////////////////

            if(q=='d'){
                
                ShowImg("Sailboat.raw", sailBoat);
                ShowImg("Color vector F-S error diffusion 'Sailboat.raw'",VectorColorHalftoning(sailBoat));
            }

            break;
        }
        case 3:
        {
            cout<<"which sub problem, enter a,b:"<<endl;
            char q;
            cin>>q;
            
            //////////////////////////////////////////////////////////
            //Problem 3 (a)
            //////////////////////////////////////////////////////////
            
            if(q=='a'){
                const char* HORSESHOE="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P3/Horseshoe.raw";
                imageData horseShoe(HORSESHOE,91,108,1);
                ShowImg("Horseshoe.raw", horseShoe);
                ShowImg("shrinked Horseshoe showing nails in white", Morphing(horseShoe,1,0));
                ShowImg("shrinked Horseshoe showing holes in white", Morphing(horseShoe,1,1));
                ShowImg("First-pass labels", ConectedLabeling(horseShoe));
                
            }
            
            //////////////////////////////////////////////////////////
            //Problem 3 (b)
            //////////////////////////////////////////////////////////
            
            if(q=='b'){
                const char* HORSE1="/Users/liuhengyue/Google Drive/EE569/hw3/HW3_images/P3/Horse1.raw";
                imageData horse1(HORSE1,360,275,1);
                ShowImg("horse1.raw", horse1);
                //ShowImg("Thined horse", Morphing(horse1,2,0));
                imageData horse2=horse1.copy();
                ShowImg("shrinked horse", Morphing(horse1,1,0));
                ShowImg("Thined and skeletonized horse", Morphing(Morphing(horse1,2,0),3,0));
                //note boundary smoothing is embedded
                ShowImg("pre-processed horse", HoleFilling(horse1,0) );
                ShowImg("Thined and skeletonized horse with pre-process", Morphing(Morphing(HoleFilling(horse1,0),2,0),3,0));



            }
            
            
            break;
        }
        default:
            break;
    }
    

    return 0;
}
