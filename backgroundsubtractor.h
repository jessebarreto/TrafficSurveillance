#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#include "virtualdetection.h"
#include "package_bgs/IBGS.h"

class BackgroundSubtractor : public VirtualDetection
{
    IBGS *_bgs;

    cv::Mat _imgBgkModel;

public:
    BackgroundSubtractor(IBGS *bgRef);

    ~BackgroundSubtractor();

    // Process Identification Method
    void process(const cv::Mat &frame, cv::Mat &result);

    // Initialization
    void init();
};

#endif // BACKGROUNDSUBTRACTOR_H
