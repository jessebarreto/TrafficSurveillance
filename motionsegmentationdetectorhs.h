#ifndef MOTIONSEGMENTATIONDETECTORHS_H
#define MOTIONSEGMENTATIONDETECTORHS_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "virtualdetection.h"
#include "flowdrawer.h"

class MotionSegmentationDetectorHS : public VirtualDetection, protected FlowDrawer
{
private:
    // Algorithm Parameters
    double _alpha;
    int _morphBoxSize;

    // Stores if process just started
    bool _startFlag;
    bool _showFlowFrameFlag;

    // Stores Previous Frame
    cv::Mat _prevFrame;

    // Image of the Flow
    cv::Mat _flowImage;

    // Windows' Names
    std::string _flowWinName;


    void _calculateFlowHS(const cv::Mat &current, const cv::Mat &previous,
                          cv::Mat &flow, double alpha, int iterations = 200);

    void _filterFlow(const cv::Mat &flows, cv::Mat &filteredFlows);

    void _mergeFlows(const cv::Mat &flowX, const cv::Mat &flowY, cv::Mat &flowXY);

    void _splitFlows(const cv::Mat &flowXY, cv::Mat &flowX, cv::Mat &flowY);

public:
    MotionSegmentationDetectorHS(double alpha, int morphBoxSize);

    void process(const cv::Mat &frame, cv::Mat &result) override;
};

#endif // MOTIONSEGMENTATIONDETECTORHS_H
