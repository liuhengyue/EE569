% EE569 Homework Assignment #1
% Date: September 20, 2015
% Name: Hengyue Liu
% ID: 4107-2966-75
% email: hengyuel@usc.edu
function resizedimg=bilinear_interpolation(imgmatrix,N)
%bilinear_interpolation - read a color image square matrix, and do the
%bilinear interpolation based on the desired image size N*N
% Usage:	 resizedimg=bilinear_interpolation(imgmatrix,N)
n=length(imgmatrix);%get the original size of n*n matrix
resizedimg=zeros(N,N,3);
    for row=1:N
        for col=1:N
            %n/N is the shrink ratio from the desied image to the original
            %one, (u, v) are the shrinked coordinates, (x, y) are the
            %round-off of (u, v), a and b are the weight values.
            u=row*n/N; v=col*n/N; x=floor(u); y=floor(v);a=u-x;b=v-y;
            % The round-off values may be out of boundries, so make them in
            % the right range n.
            if x<1
                x=1;
            elseif x>n-1
                x=n-1;
            end
            if y<1
                y=1;
            elseif y>n-1
                y=n-1;
            end
            %Bilinear Interpolation, F(row,col) is decided by F(x,y),F(x,y+1),F(x+1,y) and F(x+1,y+1) 
            for chann=1:3
                resizedimg(row,col,chann)=(1-a)*(1-b)*imgmatrix(x,y,chann)+b*(1-a)*imgmatrix(x,y+1,chann)+a*(1-b)*imgmatrix(x+1,y,chann)+a*b*imgmatrix(x+1,y+1,chann);
                
            end
        end
    end
    resizedimg=uint8(resizedimg);%The matrix's precison is double, change it to uint8 to display.
    imshow(resizedimg);
    end %function