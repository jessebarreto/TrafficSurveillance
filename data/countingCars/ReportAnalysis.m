% Data colected from Tracking Cars
numberOfVideos = 14;

% Number of Identificators - 4
% * 2    - BGS Mean of Gaussians v2 (It works better than v1)
% * 3    - BGS Multilayer algorithm.
% * 4    - BGS Pixel Based Adaptative Segmenter algorithm.
nIdentificators = 4;

% Number of Detectors
%  * 0    - Weighted Moving Average Filter
%  * 1    - Kalman Filter
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

% ProcessTime
processTimeValues = zeros(numberOfVideos, nIdentificators, nDetectors);

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
% Exp MOG-Avg
morphValues(curVideo, 1, 1)         = 5;        % morph
carsCountedTrue(curVideo, 1, 1)     = 10;       % CountTrue
carsCountedFalse(curVideo, 1, 1)    = 0;        % CountFalse
carsNotCountedTrue(curVideo, 1, 1)  = 0;        % NotCountTrue
processTimeValues(curVideo, 1, 1)   = 25.51;    % ProcessTime
% Exp MOG-Kalman
morphValues(curVideo, 1, 2)         = 5;    % morph
carsCountedTrue(curVideo, 1, 2)     = 9;    
carsCountedFalse(curVideo, 1, 2)    = 0;
carsNotCountedTrue(curVideo, 1, 2)  = 1;
processTimeValues(curVideo, 1, 1)   = 26.98;    % ProcessTime
% Exp Multilayer-Avg
morphValues(curVideo, 2, 1)         = 1;    % morph
carsCountedTrue(curVideo, 2, 1)     = 9;
carsCountedFalse(curVideo, 2, 1)    = 0;
carsNotCountedTrue(curVideo, 2, 1)  = 1;
processTimeValues(curVideo, 1, 1)   = 28.32;    % ProcessTime
% Exp Multilayer-Kalman
morphValues(curVideo, 2, 2)         = 1;    % morph
carsCountedTrue(curVideo, 2, 2)     = 9;
carsCountedFalse(curVideo, 2, 2)    = 0;
carsNotCountedTrue(curVideo, 2, 2)  = 1;
processTimeValues(curVideo, 1, 1)   = 29.89;    % ProcessTime
% Exp PBAS-Avg
morphValues(curVideo, 3, 1)         = 1;    % morph
carsCountedTrue(curVideo, 3, 1)     = 6;
carsCountedFalse(curVideo, 3, 1)    = 1;
carsNotCountedTrue(curVideo, 3, 1)  = 5;
processTimeValues(curVideo, 1, 1)   = 49.16;    % ProcessTime
% Exp PBAS-Kalman
morphValues(curVideo, 3, 2)         = 1;    % morph
carsCountedTrue(curVideo, 3, 2)     = 7;
carsCountedFalse(curVideo, 3, 2)    = 0;
carsNotCountedTrue(curVideo, 3, 2)  = 3;
processTimeValues(curVideo, 1, 1)   = 50.78;    % ProcessTime

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

