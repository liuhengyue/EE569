% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function count = writeraw(G, filename, chann)
%writeraw - write RAW format grey scale or RGB image file 
% Usage :	writeraw(G, filename, chann)
% G:		input image matrix
% filename: file name of the file to write to disk
% count:	return value, the elements written to file

	disp([' Write image data to'  filename ' ...']);

	% Get file ID
	fid = fopen(filename,'wb');

	% Check if file exists
	if (fid == -1)
		error('can not open output image filem press CTRL-C to exit \n');
		pause
	end

	if chann==1
        % Transpose matrix to write file properly
        G = G'; 

        % Write and close file
        count = fwrite(fid,G, 'uchar');
        fclose(fid);

        % Transpose again to restore matrix
        G = G';
    end
    if chann==3 %if RGB image
        for chann_index=1:chann
            sub_G=G(:,:,chann_index);
            G(:,:,chann_index)=sub_G';
        end
                % Write and close file
        count = fwrite(fid,G, 'uchar');
        fclose(fid);

        % Transpose again to restore matrix
        for chann_index=1:chann
            G(:,:,chann_index)=G(:,:,chann_index)';
        end
    end
end %function