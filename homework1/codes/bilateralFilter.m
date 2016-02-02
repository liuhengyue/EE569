% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function filteredImg=bilateralFilter(imgMatrix,windowSize)
%medianFilter- read a grey scale image matrix, and do the N-point median
%filter to the image. N = windowSize.
% Usage:	 filteredImg=medianFilter(imgMatrix,windowSize)
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
%Get image size
[width,length]=size(imgMatrix);
%Construct the output matrix
filteredImg=zeros(width,length);
%Construct a temporary enlarged matrix for boundary problem.
tempImg=imgMatrix;
%Window size is incremented by 4 points each time.
addedRow=mod(windowSize,4);
for mirrorTime=1:addedRow
    tempImg=mirror(tempImg);
end
%Bilateral Filter window coefficients
filterCoeff=zeros(width,length);

for row=1:width
    for col=1:length
        neighbors=zeros(2,windowSize);
        for i=-addedRow:addedRow
            for j=-addedRow:addedRow
                for neighbor=1:windowSize
                    neighbors(1,neighbor)=sqrt(i^2+j^2);
                    neighbors(2,neighbor)=(tempImg(row+addedRow,col+addedRow)-tempImg(row+addedRow+i,col+addedRow+j));
                end
            end
        end
        %calculate variances
        varDistance=var(neighbors(1,:));
        varIntensity=var(neighbors(2,:));
        for i=-addedRow:addedRow
            for j=-addedRow:addedRow
                %Get weight values for each pixel
                filterCoeff(row,col)=exp(-(i^2+j^2)/(2*varDistance))*exp(-(tempImg(row+addedRow,col+addedRow)-tempImg(row+addedRow+i,col+addedRow+j))^2/(2*varIntensity));
            end
        end
        filterCoeff(row,col)=filterCoeff(row,col)/(windowSize-1);
    end
end

for mirrorTime=1:addedRow
    filterCoeff=mirror(filterCoeff);
end
%local summation mean
for row=1:width
    for col=1:length
        for i=-addedRow:addedRow
            for j=-addedRow:addedRow
                %Get filtered value of each pixel
                filteredImg(row,col)=filterCoeff(row+addedRow+i,col+addedRow+j)*tempImg(row+addedRow+i,col+addedRow+j);
            end
        end
    end
end

end%function