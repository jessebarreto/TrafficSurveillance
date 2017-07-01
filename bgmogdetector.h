#ifndef BGMOGDETECTOR_H
#define BGMOGDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "virtualdetection.h"

class BGMOGDetector : public VirtualDetection
{
private:
    // Algorithm Parameters
    double _learningRate;
    int _thresholdValue;
    int _morphboxSize;

    // MOG2 Background Subtractor
    cv::Ptr<cv::BackgroundSubtractor> _ptrSubtractor;

    // Stores if process just started
    bool _startFlag;
    bool _showFGMask;
    bool _showBGMask;
    bool _showBinarizedFGMask;
    bool _showMorphFGMask;

    // Background Mask
    cv::Mat _bgMask;

    // Foreground Mask
    cv::Mat _fgMask;

    // Window
    std::string _fgMaskWinName, _bgMaskWinName, _binarizedFGMaskWinName;
    std::string _morphFGMaskWinName;

    void _morphFilter(const cv::Mat &src, cv::Mat &dst, int operation, int structElementSize);

    static void _filterParamMorphSettingHandler(int value, void *data);
public:
    BGMOGDetector(double learningRate, int thresholdValue, int morphBoxSize);

    void process(const cv::Mat &frame, cv::Mat &result) override;

    void init();
};

#endif // BGMOGDETECTOR_H
