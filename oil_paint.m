% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function [I_1]=oil_paint(imgmatrix,value_num)
%oil_paint - Read the image matrix, do the oil paint effect to the
%image, return the enhanced image. value_num is the quantization level
%Usage: [I_1]=oil_paint(imgmatrix,value_num)
%Get the matrix size
[Y,X,chann]=size(imgmatrix);
%Total numbers of values in one channel
Size=Y*X;
% value_num is equal to quantization level, 
%Step is for chopping the array into equal lengths, having 4 quantization levels
%means all pixels are only in 4 values.
step=floor(Size/value_num);
%Construct an array to store quantized colors
quantized_color=zeros(1,value_num,chann);
%Construct an array to store quantized thresholds
thresholds=zeros(1,value_num,chann);
%Construct an quantized image matrix
I_1=zeros(Y,X,chann);
%Construct a transfer function
transfer_function=zeros(1,256,chann);
%Get the histogram of the original image
intensity_values=histogram_(imgmatrix);
%Calculate the cumulative distribution function of the histogram
for chann_index=1:chann
    for level=1:256%find the transfer value for each level
        for i=1:level 
            transfer_function(1,level,chann_index)=transfer_function(1,level,chann_index)+intensity_values(1,i,chann_index);
        end
    end
    for quantize=1:value_num
        diff=Size;
        threshold=0;
        for level=1:256
            %Group four bins of pixel values
            if abs(transfer_function(1,level,chann_index)- Size/(value_num-quantize+1)) <diff
                diff=abs(transfer_function(1,level,chann_index)- Size/(value_num-quantize+1));
                threshold=level-1;%This is the threshold value for first 1/4 pixels
            end
        end
        thresholds(1,quantize,chann_index)=threshold;
        quantized_color(1,quantize,chann_index)=256*quantize/value_num-1;
    end
                
end

%Construct an array for sorting the pixel values
img_array=zeros(1,Y*X,chann);
sorted_array=zeros(1,Y*X,chann);
average_color=zeros(1,value_num,chann);
n=zeros(1,4);
for chann_index=1:chann
    for i=2:Size+1
        %Change 2d matrix to 1d array
        img_array(1,i-1,chann_index)=imgmatrix(ceil((i-1)/X),mod(i-2,X)+1,chann_index);
    end
    %Use sort() function to sort the pixels and record the coordinates in
    %array location.
    [sorted_array(1,:,chann_index), location]=sort(img_array(1,:,chann_index));
    %Find the average color of four bins.
 
    for value=1:Size
        %Change the intesity values
        if  sorted_array(1,value,chann_index) <= thresholds(1,1,chann_index)
            average_color(1,1,chann_index)=average_color(1,1,chann_index)+sorted_array(1,value,chann_index);
            n(1,1)=n(1,1)+1;
            %sorted_array(1,value,chann_index)=quantized_color(1,level+1,chann_index);
        elseif thresholds(1,1,chann_index) < sorted_array(1,value,chann_index) <= thresholds(1,2,chann_index)
            average_color(1,2,chann_index)=average_color(1,2,chann_index)+sorted_array(1,value,chann_index);
            n(1,2)=n(1,2)+1;
        elseif thresholds(1,2,chann_index) < sorted_array(1,value,chann_index) <= thresholds(1,3,chann_index)
            average_color(1,3,chann_index)=average_color(1,3,chann_index)+sorted_array(1,value,chann_index);
            n(1,3)=n(1,3)+1;
        elseif thresholds(1,3,chann_index) < sorted_array(1,value,chann_index) <= thresholds(1,4,chann_index)
            average_color(1,4,chann_index)=average_color(1,4,chann_index)+sorted_array(1,value,chann_index);
            n(1,1)=n(1,4)+1;
        else
            error('calculate average color wrong');
        end

    end
    for i=1:4
        average_color(1,i,chann_index)=average_color(1,i,chann_index)/n(1,i);
    end

% Change the intensity values
    for value=1:Size
        %Change the intesity values
        if  sorted_array(1,value,chann_index) <= thresholds(1,1,chann_index)
            sorted_array(1,value,chann_index)=average_color(1,1,chann_index);
        elseif thresholds(1,1,chann_index) < sorted_array(1,value,chann_index) <= thresholds(1,2,chann_index)
            sorted_array(1,value,chann_index)=average_color(1,2,chann_index);
        elseif thresholds(1,2,chann_index) < sorted_array(1,value,chann_index) <= thresholds(1,3,chann_index)
            sorted_array(1,value,chann_index)=average_color(1,3,chann_index);
        else
            sorted_array(1,value,chann_index)=average_color(1,4,chann_index);
        end

    end
    for i=1:Size
        %Recover the image array from value-changed sorted value array.
        img_array(1,location(1,i),chann_index)=sorted_array(1,i,chann_index);
    end
    %Recover the imgmatrix
    sub_channel=zeros(X,Y);
    sub_channel(1:Size)=img_array(1,1:Size,chann_index);
    I_1(:,:,chann_index)=sub_channel';
    
end
disp(average_color);
end%function