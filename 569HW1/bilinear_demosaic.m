% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function clearimg=bilinear_demosaic(imgmatrix)
%bilinear_demosaic - read a grey scale image matrix, 
%and do the bilinear demosaicing and create a new image matrix
% Usage:	 clearimg=bilinear_demosaic(imgmatrix)
[Y, X]=size(imgmatrix);%get the original size of Y*X matrix
%Extra 2 rows and 2 columns are for dealing with the boundary problem.
%Reason: Local averaging at most 4 pixels, so 2 more rows and columns are enough.
%copy the pixels and do the mirror reflection to create a temporary matrix.
tempimg=zeros(Y+2,X+2);
tempimg(1,1)=imgmatrix(2,2);
tempimg(1,X+2)=imgmatrix(2,X-1);
tempimg(Y+2,1)=imgmatrix(Y-1,2);
tempimg(Y+2,X+2)=imgmatrix(Y-1,X-1);
tempimg(1,2:X+1)=imgmatrix(2,1:X);
tempimg(2:Y+1,1)=imgmatrix(1:Y,2);
tempimg(Y+2,2:X+1)=imgmatrix(Y-1,1:X);
tempimg(2:Y+1,X+2)=imgmatrix(1:Y,X-1);
tempimg(2:Y+1,2:X+1)=imgmatrix(1:Y,1:X);
clearimg=zeros(Y,X,3);
%Tempimg coordinates(Y,X) are 1 bigger than clearimg, so adjust the coordinates
%carefully.
%Red pixel estimate G and B
    for row=1:2:Y
        for col=1:2:X
            clearimg(row,col,1)=imgmatrix(row,col);
            clearimg(row,col,2)=(tempimg(row,col+1)+tempimg(row+1,col)+tempimg(row+2,col+1)+tempimg(row+1,col+2))/4;
            clearimg(row,col,3)=(tempimg(row,col)+tempimg(row+2,col)+tempimg(row,col+2)+tempimg(row+2,col+2))/4;
        end
    end
%Green pixel estimate R and B
%Green pixel at red rows
    for row=1:2:Y
        for col=2:2:X
            clearimg(row,col,1)=(tempimg(row+1,col)+tempimg(row+1,col+2))/2;
            clearimg(row,col,2)=imgmatrix(row,col);
            clearimg(row,col,3)=(tempimg(row,col+1)+tempimg(row+2,col+1))/2;
        end
    end
%Green pixel at blue rows
    for row=2:2:Y
        for col=1:2:X
            clearimg(row,col,1)=(tempimg(row,col+1)+tempimg(row+2,col+1))/2;
            clearimg(row,col,2)=imgmatrix(row,col);
            clearimg(row,col,3)=(tempimg(row+1,col)+tempimg(row+1,col+2))/2;
    end
%Blue pixel estimate R and G
    for row=2:2:Y
        for col=2:2:X
            clearimg(row,col,1)=(tempimg(row,col)+tempimg(row+2,col)+tempimg(row,col+2)+tempimg(row+2,col+2))/4;
            clearimg(row,col,2)=(tempimg(row,col+1)+tempimg(row+1,col)+tempimg(row+2,col+1)+tempimg(row+1,col+2))/4;
            clearimg(row,col,3)=imgmatrix(row,col);
        end
    end
    
    %imshow(uint8(clearimg));
    end %function