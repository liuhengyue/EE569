% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function filteredImg=medianFilter(imgMatrix)
%medianFilter- read a grey scale image matrix, and do the 9-point median
%filter to the image.
% Usage:	 filteredImg=medianFilter(imgMatrix)
%Define a function mirror() to add extra 2 rows and columns (for grey scale image), to add 4
%ones, just run the function 2 times.
% Usage:	 temimg=mirror(imgmatrix)
    function tempimg=mirror(imgMatrix)
        [Y X]=size(imgMatrix);%get the original size of Y*X matrix
        tempimg=zeros(Y+2,X+2);
        tempimg(1,1)=imgMatrix(2,2);
        tempimg(1,X+2)=imgMatrix(2,X-1);
        tempimg(Y+2,1)=imgMatrix(Y-1,2);
        tempimg(Y+2,X+2)=imgMatrix(Y-1,X-1);
        tempimg(1,2:X+1)=imgMatrix(2,1:X);
        tempimg(2:Y+1,1)=imgMatrix(1:Y,2);
        tempimg(Y+2,2:X+1)=imgMatrix(Y-1,1:X);
        tempimg(2:Y+1,X+2)=imgMatrix(1:Y,X-1);
        tempimg(2:Y+1,2:X+1)=imgMatrix(1:Y,1:X);
    end

end%function