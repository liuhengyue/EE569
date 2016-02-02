function colorimg = ReadRaw(filename,width,length,chann)
%readcolorraw - read RAW format color image of square size into matrix
%colorimg
% Usage:	colorimg = readcolorraw(filename)

	disp(['	Retrieving RGB Color Image ' filename ' ...']);

	% Get file ID for file
	fid=fopen(filename,'rb');

	% Check if file exists
	if (fid == -1)
	  	error('can not open input image file press CTRL-C to exit \n');
	  	pause
	end

	% Get all the pixels from the image
	pixel = fread(fid, inf,'uchar');

	% Close file
	fclose(fid);
    Size=length*width*chann;
    if chann==1
        colorimg=zeros(width,length,'uint8');
        colorimg(1:Size) = pixel(1:Size);
        colorimg=colorimg';
    end
    if chann==3
        %N=sqrt(Size/3);%Since the image has three channels, Size/3 is the image size.
        
        %N=length*width;
        % Construct matrix
        R = zeros(width,length);
        G = zeros(width,length);
        B = zeros(width,length);
        colorimg=zeros(length,width,3,'uint8');
        % Write pixels into matrix
        R(1:Size/3) = pixel(1:3:Size);
        G(1:Size/3) = pixel(2:3:Size);
        B(1:Size/3) = pixel(3:3:Size);
        % Transpose matrix, to orient it properly
        R = R';
        G = G';
        B = B';
        colorimg(1:length,1:width,1)=R;
        colorimg(1:length,1:width,2)=G;
        colorimg(1:length,1:width,3)=B;
    end
    
end %function
