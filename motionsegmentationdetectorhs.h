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
    int _alphaChangeable;

    // Stores if process just started
    bool _startFlag;
    bool _showFlowFrameFlag;
    bool _showMagFlowFlag;
    bool _showBinaryMagFlowFlag;
    bool _showMorphFlag;

    // Stores Previous Frame
    cv::Mat _prevFrame;

    // Image of the Flow
    cv::Mat _flowImage, _filteredFlowImage;

    // Windows' Names
    std::string _flowWinName;
    std::string _filteredFlowWinName;
    std::string _magFlowWinName;
    std::string _binaryMagFlowWinName;
    std::string _morphWinName;

    int _maxAlpha;

    void _calculateFlowHS(const cv::Mat &current, const cv::Mat &previous,
                          cv::Mat &flow, double alpha, int iterations = 200);

    void _filterFlow(const cv::Mat &flows, cv::Mat &filteredFlows, bool adaptative);

    void _mergeFlows(const cv::Mat &flowX, const cv::Mat &flowY, cv::Mat &flowXY);

    void _splitFlows(const cv::Mat &flowXY, cv::Mat &flowX, cv::Mat &flowY);

    void _morphFilter(const cv::Mat &binaryMagFlow, cv::Mat &segmentedFlow, int operation, int structElementSize);

    static void _filterParamMorphSettingHandler(int value, void *data);

    static void _filterParamAlphaSettingHandler(int value, void *data);

public:
    MotionSegmentationDetectorHS(double alpha, int morphBoxSize);

    void process(const cv::Mat &frame, cv::Mat &result) override;

    void init() override;

    void setMorphSize(int morphSize) override;
};

#endif // MOTIONSEGMENTATIONDETECTORHS_H
