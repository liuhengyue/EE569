
function[F,V]=RFDetector()
%% set opts for training (see edgesTrain.m)
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory

%% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
tic, model=edgesTrain(opts); toc; % will load model if already trained

%% set detection parameters (can set after training)
model.opts.multiscale=0;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=4;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=0;                 % set to true to enable nms

%% evaluate edge detector on BSDS500 (see edgesEval.m)
if(0), edgesEval( model, 'show',1, 'name','' ); end

%% detect edge and visualize results
%Read raw images and apply RF detector
I = ReadRaw('C:\Users\liuhe\Google Drive\EE569\hw2\HW2_images\P2\Farm.raw',481,321,3);
tic, E=edgesDetect(I,model); toc
figure(1); im(I); figure(2); im(1-E);

G=struct('groundTruth',[]);
G.groundTruth=cell(1,5);
for g=1:5
    G.groundTruth{g}=struct('Boundaries',[]);
end
%flip the values
G.groundTruth{1,1}.Boundaries=(1-int8(ReadRaw('C:\Users\liuhe\Google Drive\EE569\hw2\HW2_images\P2\Farm_GT1.raw',481,321,1)/255));
G.groundTruth{1,2}.Boundaries=(1-int8(ReadRaw('C:\Users\liuhe\Google Drive\EE569\hw2\HW2_images\P2\Farm_GT2.raw',481,321,1)/255));
G.groundTruth{1,3}.Boundaries=(1-int8(ReadRaw('C:\Users\liuhe\Google Drive\EE569\hw2\HW2_images\P2\Farm_GT3.raw',481,321,1)/255));
G.groundTruth{1,4}.Boundaries=(1-int8(ReadRaw('C:\Users\liuhe\Google Drive\EE569\hw2\HW2_images\P2\Farm_GT4.raw',481,321,1)/255));
G.groundTruth{1,5}.Boundaries=(1-int8(ReadRaw('C:\Users\liuhe\Google Drive\EE569\hw2\HW2_images\P2\Farm_GT5.raw',481,321,1)/255));
%Important note: G=load(G) is delete in the original edgesEvalImg.m
%function
%calculate F-mearsure
[thrs,cntR,sumR,cntP,sumP,V] = edgesEvalImg( E, G);
R = cntR./sumR;
P = cntP./sumP;
F=mean(2*R.*P./(R+P));
%I = ReadColorRaw('C:\Users\liuhe\Google Drive\EE569\hw2\HW2_images\P2\Cougar.raw',481,321);
%tic, E=edgesDetect(I,model); toc
%figure(3); im(I); figure(4); im(1-E);
end
