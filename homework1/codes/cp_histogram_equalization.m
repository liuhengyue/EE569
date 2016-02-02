% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function [enhancedimg]=cp_histogram_equalization(imgmatrix)
%cp_histogram_equalization - Read the image matrix, do the cumulative-histogram-based histogram equalization to the
%image, return the enhanced image.
%Usage: [enhancedimg, transfer_function]=cp_histogram_equalization(imgmatrix,intensity_values)
%Get the matrix size
[Y,X,chann]=size(imgmatrix);
%Total numbers of values in one channel
Size=Y*X;
%Step is for chopping the array into equal lengths
step=floor(Size/256);
%Construct a enhanced image matrix
enhancedimg=zeros(Y,X,chann);
%Construct an array for sorting the pixel values
img_array=zeros(1,Y*X,chann);
sorted_array=zeros(1,Y*X,chann);
for chann_index=1:chann
    for i=2:Size+1
        %Change 2d matrix to 1d array
        img_array(1,i-1,chann_index)=imgmatrix(ceil((i-1)/X),mod(i-2,X)+1,chann_index);
    end
    %Use sort() function to sort the pixels and record the coordinates in
    %array location.
    [sorted_array(1,:,chann_index), location]=sort(img_array(1,:,chann_index));
    %For each level, assign step number of pixels to this level, and the
    %equal durations are like 1:step, 1+step:2step, ...
    for level=0:255
        for value=level*step+1:(level+1)*step
            %Change the intesity values
            sorted_array(1,value,chann_index)=level;
        end
    end
    for i=1:Size
        %Recover the image array from value-changed sorted value array.
        img_array(1,location(1,i),chann_index)=sorted_array(1,i,chann_index);
    end
    %Recover the imgmatrix
    sub_channel=zeros(X,Y);
    sub_channel(1:Size)=img_array(1,1:Size,chann_index);
    enhancedimg(:,:,chann_index)=sub_channel';
    
end

end%function