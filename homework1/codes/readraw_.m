% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function G = readraw_(filename,width,length,chann)
%readraw_ - read RAW format grey scale or RGB image of any size into matrix G
% Usage:	G = readraw_(filename,width,length,chann)
%Warning: width and length are not required. Channels number need to be
%right,though error module is designed.

	disp(['	Retrieving Image ' filename ' ...']);

	% Get file ID for file
	fid=fopen(filename,'rb');

	% Check if file exists
	if (fid == -1)
	  	error('can not open input image file press CTRL-C to exit \n');
	  	pause
    end
    % Check if the channels number is right
    if uint8(chann)~=1 && uint8(chann)~=3
        error('channels number wrong press CTRL-C to exit \n');
	  	pause
    end
	% Get all the pixels from the image
	pixel = fread(fid, inf, 'uchar');

	% Close file
	fclose(fid);

	% Calculate the size of image
	[Y X]=size(pixel);
	Size=(Y*X);
    %Check if the image channels and image mode are match

    %The pixel is N-D array, so it is necessary to find a proper image
    %size(Y*X) if the dimensions are not given; here use the function mod to find the value can be divided by
    %Size.
    if width==0 || length==0
        N=ceil(sqrt(Size/chann));
        while mod(Size,N)>0
            N=N+1;
        end
    length=N;
    width=Size/chann/N;
    end
	% Construct matrix
    G = zeros(length,width,chann);
	% Write pixels into matrix
    try
        for chann_index=1:chann
            sub_G=zeros(width,length);
            sub_G(1:length*width)=pixel(chann_index:chann:Size);
            % Assign channels and transpose matrix, to orient it properly
            % since array stores in columns
            G(1:length,1:width,chann_index)=sub_G';

        end

    catch
        error('channels number mismatch with the image color level press CTRL-C to exit \n');
	  	pause
    end

end %function
