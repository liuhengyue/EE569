% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function intensity_values=histogram_(imgmatrix)
%histogram_ - Read the image matrix and plot the histograms of three
%channels; return the intensity value array.
%Usage: hist=histogram_(imgmatrix)
%Get the matrix size
[Y,X,chann]=size(imgmatrix);
%This matrix is for recording intensity values of each level
%Find the value for each pixel, and add one at the corresponding location of this matrix
intensity_values=zeros(1,256,chann);
for chann_index=1:chann
    for row=1:Y
        for col=1:X
            %imgmatrix(row,col,chann_index) should be in 0-255, so add 1 to
            %adjust the matrix indices.
            intensity_values(1,imgmatrix(row,col,chann_index)+1,chann_index)=intensity_values(1,imgmatrix(row,col,chann_index)+1,chann_index)+1;
        end
    end
end
plot(0:255,intensity_values(:,:,1),'r',0:255,intensity_values(:,:,2),'g',0:255,intensity_values(:,:,3),'b');axis([0 255 0 inf]);legend('R','G','B');
end