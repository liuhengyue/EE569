function count = writeraw(G, filename, chann)
%writeraw - write RAW format grey scale or RGB image file 
% Usage :	writeraw(G, filename, chann)
% G:		input image matrix
% filename: file name of the file to write to disk
% count:	return value, the elements written to file

function G = readraw_(filename,width,length,chann)
%readraw_ - read RAW format grey scale or RGB image of any size into matrix G
% Usage:	G = readraw_(filename,width,length,chann)
%Warning: width and length are not required. Channels number need to be
%right,though error module is designed.

function resizedimg=bilinear_interpolation(imgmatrix,N)
%bilinear_interpolation - read a color image square matrix, and do the
%bilinear interpolation based on the desired image size N*N
% Usage:	 resizedimg=bilinear_interpolation(imgmatrix,N)

function clearimg=bilinear_demosaic(imgmatrix)
%bilinear_demosaic - read a grey scale image matrix, 
%and do the bilinear demosaicing and create a new image matrix
% Usage:	 clearimg=bilinear_demosaic(imgmatrix)
%Extra 2 rows and 2 columns are for dealing with the boundary problem.
%Reason: Local averaging at most 4 pixels, so 2 more rows and columns are enough.
%copy the pixels and do the mirror reflection to create a temporary matrix.

function clearimg=MHC_demosaic(imgmatrix)
%MHC_demosaic - read a grey scale image matrix, and do the
%MHC demosaicing and create a new image matrix
% Usage:	 clearimg=MHC_demosaic(imgmatrix)

function tempimg=mirror(imgmatrix)
%Extra 4 rows and 4 columns are for dealing with the boundary problem.
%Reason: At most 9-point Laplacian, so 4 more rows and columns are enough.
%copy the pixels and do the mirror reflection to create a temporary matrix.
%Define a function mirror() to add extra 2 rows and columns, to add 4
%ones, just run the function 2 times.
% Usage:	 temimg=mirror(imgmatrix)

function intensity_values=histogram_(imgmatrix)
%histogram_ - Read the image matrix and plot the histograms of three
%channels; return the intensity value array.
%Usage: hist=histogram_(imgmatrix)

function [enhancedimg, transfer_function]=tf_histogram_equalization(imgmatrix,intensity_values)
%tf_histogram_equalization - Read the image matrix and the results of
%histogram_, do the transfer-function-based histogram equalization to the
%image, return the enhanced image and transfer functions.
%Usage: [enhancedimg, transfer_function]=tf_histogram_equalization(imgmatrix,intensity_values)

function [enhancedimg]=cp_histogram_equalization(imgmatrix)
%cp_histogram_equalization - Read the image matrix, do the cumulative-histogram-based histogram equalization to the
%image, return the enhanced image.
%Usage: [enhancedimg, transfer_function]=cp_histogram_equalization(imgmatrix,intensity_values)
%Get the matrix size

function [I_1]=oil_paint(imgmatrix,value_num)
%oil_paint - Read the image matrix, do the oil paint effect to the
%image, return the enhanced image. value_num is the quantization level
%Usage: [I_1]=oil_paint(imgmatrix,value_num)

function filteredImg=medianFilter(imgMatrix)
%medianFilter- read a grey scale image matrix, and do the 9-point median
%filter to the image.
% Usage:	 filteredImg=medianFilter(imgMatrix)

function filteredImg=bilateralFilter(imgMatrix,windowSize)
%bilateralFilter- read a grey scale image matrix, and do the N-point bilateral
%filter to the image. N = windowSize.
% Usage:	 filteredImg=bilateralFilter(imgMatrix,windowSize)