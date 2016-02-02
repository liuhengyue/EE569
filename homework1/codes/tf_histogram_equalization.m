% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function [enhancedimg, transfer_function]=tf_histogram_equalization(imgmatrix,intensity_values)
%tf_histogram_equalization - Read the image matrix and the results of
%histogram_, do the transfer-function-based histogram equalization to the
%image, return the enhanced image and transfer functions.
%Usage: [enhancedimg, transfer_function]=tf_histogram_equalization(imgmatrix,intensity_values)
%Get the matrix size
[Y,X,chann]=size(imgmatrix);
%Total numbers of values in one channel
Size=Y*X;
%Construct a transfer function
transfer_function=zeros(1,256,chann);
%Construct a enhanced image matrix
enhancedimg=zeros(Y,X,chann);
%Calculate the cumulative distribution function of the histogram
for chann_index=1:chann
    for level=1:256%find the transfer value for each level
        for i=1:level 
            transfer_function(1,level,chann_index)=transfer_function(1,level,chann_index)+intensity_values(1,i,chann_index);
        end
    end
    %normalize the transfer_function and coordinates 0-255 to 0-255
    transfer_function(1,:,chann_index)=floor(256*transfer_function(1,:,chann_index)/Size);
    
end
%Find the corresponding pixels and change their values to tranferred ones
for chann_index=1:chann
    for value=0:255
        for row=1:Y
            for col=1:X
                if imgmatrix(row,col,chann_index)==value
                    enhancedimg(row,col,chann_index)=transfer_function(1,value,chann_index);
                end
            end
        end
    end
end
%Plot the transfer functions of three channels
plot(0:255,transfer_function(:,:,1),'r',0:255,transfer_function(:,:,2),'g',0:255,transfer_function(:,:,3),'b');axis([0 255 0 255]);legend('R','G','B');
end%function