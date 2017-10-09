% Data colected from Tracking Cars
numberOfVideos = 14;

% Number of Identificators - 4
% * 1    - BGS Mean of Gaussians v2 (It works better than v1)
% * 2    - BGS Multilayer algorithm.
% * 3    - BGS Pixel Based Adaptative Segmenter algorithm.
nIdentificators = 4;

% Number of Detectors
%  * 1    - Weighted Moving Average Filter
%  * 2    - Kalman Filter
nDetectors = 2;

% Total Cars Counter
carsCounted = zeros(numberOfVideos, 1);

% Cars Counted which are cars
carsCountedTrue = zeros(numberOfVideos, nIdentificators, nDetectors);

% Cars Counted which are not cars
carsCountedFalse = zeros(numberOfVideos, nIdentificators, nDetectors);

% Cars not counbted which are cars
carsNotCountedTrue = zeros(numberOfVideos, nIdentificators, nDetectors);

% Algorithm Parametes for Every Experiment
morphValues = zeros(numberOfVideos, nIdentificators, nDetectors);
carMinSizeValues = zeros(numberOfVideos, nIdentificators, nDetectors);
carMaxSizeValues = zeros(numberOfVideos, nIdentificators, nDetectors);

% Hard Coded Number of Cars
carsCounted(1) = 10;
carsCounted(2) = 101;
carsCounted(3) = 50;
carsCounted(4) = 28;
carsCounted(5) = 17;
carsCounted(6) = 29;
carsCounted(7) = 9;
carsCounted(8) = 20;
carsCounted(9) = 12;
carsCounted(10) = 81;
carsCounted(11) = 84;
carsCounted(12) = 125;
carsCounted(13) = 35;
carsCounted(14) = 80;

% Experiment Numbers
bsMogNumber = 1;
bsMultiNumber = 2;
bsPixelNumber = 3;
avgNumber = 1;
kfNumber = 2;

%%
% Video 1
curVideo = 1;
carMinSizeValues(1) = 10;
carMaxSizeValues(1) = 50;
% Exp
morphValues(curVideo, 1, 1) = 5;
carsCountedTrue(curVideo, 1, 1) = 10;
carsCountedFalse(curVideo, 1, 1) = 0;
carsNotCountedTrue(curVideo, 1, 1) = 0;
morphValues(curVideo, 1, 2) = 5;
carsCountedTrue(curVideo, 1, 2) = 9;
carsCountedFalse(curVideo, 1, 2) = 0;
carsNotCountedTrue(curVideo, 1, 2) = 1;
morphValues(curVideo, 2, 1) = 5;
carsCountedTrue(curVideo, 2, 1) = 10;
carsCountedFalse(curVideo, 2, 1) = 0;
carsNotCountedTrue(curVideo, 2, 1) = 0;


%%
% Video 2

% Video 3

% Video 4

% Video 5

% Video 6

% Video 7

% Video 8

% Video 9

% Video 10

% Video 11

% Video 12

% Video 13

% Video 14

