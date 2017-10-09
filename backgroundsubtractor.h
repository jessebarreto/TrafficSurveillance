#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#include "virtualdetection.h"
#include "package_bgs/IBGS.h"

class BackgroundSubtractor : public VirtualDetection
{
    IBGS *_bgs;

    cv::Mat _imgBgkModel;

    int _thresholdValue;

    int _morphBoxSize;

    void _morphFilter(const cv::Mat &src, cv::Mat &dst, int operation, int structElementSize);

public:
    BackgroundSubtractor(IBGS *bgRef);

    ~BackgroundSubtractor();

    // Process Identification Method
    void process(const cv::Mat &frame, cv::Mat &result);

    // Initialization
    void init();

    void setMorphSize(int morphSize) override;
};

#endif // BACKGROUNDSUBTRACTOR_H
