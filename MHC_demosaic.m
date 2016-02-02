% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function clearimg=MHC_demosaic(imgmatrix)
%MHC_demosaic - read a grey scale image matrix, and do the
%MHC demosaicing and create a new image matrix
% Usage:	 clearimg=MHC_demosaic(imgmatrix)

%Extra 4 rows and 4 columns are for dealing with the boundary problem.
%Reason: At most 9-point Laplacian, so 4 more rows and columns are enough.
%copy the pixels and do the mirror reflection to create a temporary matrix.
%Define a function mirror() to add extra 2 rows and columns, to add 4
%ones, just run the function 2 times.
% Usage:	 temimg=mirror(imgmatrix)
    function tempimg=mirror(imgmatrix)
        [Y X]=size(imgmatrix);%get the original size of Y*X matrix
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
    end
    
midmatrix=bilinear_demosaic(imgmatrix);%Bilinear demosaicing results
extendedmatrix=mirror(mirror(imgmatrix));%This matrix is for MHC demosaicing
[Y_ X_]=size(imgmatrix);%get the original size of Y*X matrix
clearimg=zeros(Y_,X_,3);%Store the bilinear demosaicing results
%Extended matrix coordinates(i,j) are 2 bigger than clearimg, so adjust the coordinates
%carefully.
% MHC algorithm: G(i,j)=Gbl(i,j)+factor*delta(i,j)
%Gbl(i,j) is the bilinear demosaicing result, factors for R G B are
%alpha=1/2, beta=5/8, gamma=3/4.
%Delta(i,j) is the Laplacian result of neighbor pixels.
%Define default weight factors
alpha=1/2;
beta=5/8;
gamma=3/4;

for row=1:2:Y_
    for col=1:2:X_
        clearimg(row,col,1)=midmatrix(row,col,1);
        %G at R locations
        clearimg(row,col,2)=midmatrix(row,col,2)+(1/4)*alpha*(4*extendedmatrix(row+2,col+2)-extendedmatrix(row,col+2)-extendedmatrix(row+4,col+2)-extendedmatrix(row+2,col)-extendedmatrix(row+2,col+4));
        %B at R locations (R row, R column)
        clearimg(row,col,3)=midmatrix(row,col,3)+(1/4)*alpha*(6*extendedmatrix(row+2,col+2)-(3/2)*extendedmatrix(row,col+2)-(3/2)*extendedmatrix(row+4,col+2)-(3/2)*extendedmatrix(row+2,col)-(3/2)*extendedmatrix(row+2,col+4));
    end
end

for row=2:2:Y_
    for col=2:2:X_
        clearimg(row,col,3)=midmatrix(row,col,3);
        %G at B locations
        clearimg(row,col,2)=midmatrix(row,col,2)+(1/4)*gamma*(4*extendedmatrix(row+2,col+2)-extendedmatrix(row,col+2)-extendedmatrix(row+4,col+2)-extendedmatrix(row+2,col)-extendedmatrix(row+2,col+4));
        %R at B locations (B row, B column)
        clearimg(row,col,1)=midmatrix(row,col,1)+(1/4)*gamma*(6*extendedmatrix(row+2,col+2)-(3/2)*extendedmatrix(row,col+2)-(3/2)*extendedmatrix(row+4,col+2)-(3/2)*extendedmatrix(row+2,col)-(3/2)*extendedmatrix(row+2,col+4));

    end
end

for row=1:2:Y_
    for col=2:2:X_
        clearimg(row,col,2)=midmatrix(row,col,2);
        %R at G locations (R row, B column)
        clearimg(row,col,1)=midmatrix(row,col,1)+(1/8)*beta*(5*extendedmatrix(row+2,col+2)+extendedmatrix(row,col+2)/2+extendedmatrix(row+4,col+2)/2-extendedmatrix(row+2,col)-extendedmatrix(row+2,col+4)-extendedmatrix(row+1,col+1)-extendedmatrix(row+1,col+3)-extendedmatrix(row+3,col+1)-extendedmatrix(row+3,col+3));
        %B at G locations (R row, B column)
        clearimg(row,col,3)=midmatrix(row,col,3)+(1/8)*beta*(5*extendedmatrix(row+2,col+2)-extendedmatrix(row,col+2)-extendedmatrix(row+4,col+2)+extendedmatrix(row+2,col)/2+extendedmatrix(row+2,col+4)/2-extendedmatrix(row+1,col+1)-extendedmatrix(row+1,col+3)-extendedmatrix(row+3,col+1)-extendedmatrix(row+3,col+3));
    end
end

for row=2:2:Y_
    for col=1:2:X_
        clearimg(row,col,2)=midmatrix(row,col,2);
        %R at G locations (B row, R column)
        clearimg(row,col,1)=midmatrix(row,col,1)+(1/8)*beta*(5*extendedmatrix(row+2,col+2)-extendedmatrix(row,col+2)-extendedmatrix(row+4,col+2)+extendedmatrix(row+2,col)/2+extendedmatrix(row+2,col+4)/2-extendedmatrix(row+1,col+1)-extendedmatrix(row+1,col+3)-extendedmatrix(row+3,col+1)-extendedmatrix(row+3,col+3));
        %B at G locations (B row, R column)
        clearimg(row,col,3)=midmatrix(row,col,3)+(1/8)*beta*(5*extendedmatrix(row+2,col+2)+extendedmatrix(row,col+2)/2+extendedmatrix(row+4,col+2)/2-extendedmatrix(row+2,col)-extendedmatrix(row+2,col+4)-extendedmatrix(row+1,col+1)-extendedmatrix(row+1,col+3)-extendedmatrix(row+3,col+1)-extendedmatrix(row+3,col+3));    
    end
end


    %imshow(uint8(clearimg));

    end %function