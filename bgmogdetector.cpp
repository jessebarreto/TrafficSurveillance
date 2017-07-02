#include "bgmogdetector.h"

#include "trafficdebug.h"
#include "trafficsurveillancecommon.h"

BGMOGDetector::BGMOGDetector(double learningRate, int thresholdValue, int morphBoxSize) :
    _learningRate(learningRate),
    _thresholdValue(thresholdValue),
    _morphboxSize(morphBoxSize),
    _startFlag(true),
    _showFGMask(true),
    _showBGMask(false),
    _showBinarizedFGMask(true),
    _showMorphFGMask(true),
    _fgMaskWinName("Foreground Mask"),
    _bgMaskWinName("Background Mask"),
    _binarizedFGMaskWinName("Foreground Binarized"),
    _morphFGMaskWinName("Morphed FG Mask")
{

}

void BGMOGDetector::process(const cv::Mat &frame, cv::Mat &result)
{
    if (_startFlag) {
        result = cv::Mat::zeros(frame.size(), CV_8UC1);

        _ptrSubtractor = new cv::BackgroundSubtractorMOG2();
        _ptrSubtractor->operator ()(frame, _fgMask, _learningRate);

        if (_showFGMask) {
            setWindow(_fgMaskWinName);
        }

        if (_showBGMask) {
            setWindow(_bgMaskWinName);
        }

        if (_showBinarizedFGMask) {
            setWindow(_binarizedFGMaskWinName);
        }

        if (_showMorphFGMask) {
            setWindow(_morphFGMaskWinName);
            cv::createTrackbar("Morph Size", _morphFGMaskWinName, &_morphboxSize, 100,
                               _filterParamMorphSettingHandler, static_cast<void *>(&_morphboxSize));
        }

        _startFlag = false;
    } else {
        // Obtain background
        _ptrSubtractor->getBackgroundImage(_bgMask);
        if (_showBGMask) {
            cv::imshow(_bgMaskWinName, _bgMask);
        }

        // Obtain foreground
        _ptrSubtractor->operator ()(frame, _fgMask, _learningRate);
        if (_showFGMask) {
            cv::imshow(_fgMaskWinName, _fgMask);
        }

        // Apply Threshold
        cv::Mat binarizedFGMask;
        cv::threshold(_fgMask, binarizedFGMask, _thresholdValue, 255, cv::THRESH_BINARY);
        if (_showBinarizedFGMask) {
            cv::imshow(_binarizedFGMaskWinName, binarizedFGMask);
        }

        // Apply Morph filter
        _morphFilter(binarizedFGMask, result, cv::MORPH_CLOSE, _morphboxSize);
        if (_showMorphFGMask) {
            cv::imshow(_morphFGMaskWinName, result);
        }
    }
}

void BGMOGDetector::init()
{

}

void BGMOGDetector::_morphFilter(const cv::Mat &src, cv::Mat &dst, int operation, int structElementSize)
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(structElementSize, structElementSize));

    if (operation == cv::MORPH_OPEN) {
        // Remove Small Blobs
        cv::Mat opening;
        cv::morphologyEx(src, opening, cv::MORPH_OPEN, kernel);

        // Fill holes
        cv::Mat closing;
        cv::morphologyEx(opening, closing, cv::MORPH_CLOSE, kernel);

        // Merge adjacent big blobs
        cv::dilate(closing, dst, kernel, cv::Point(-1,-1), 2);
    } else if (operation == cv::MORPH_CLOSE) {
        // Fill holes
        cv::Mat closing;
        cv::morphologyEx(src, closing, cv::MORPH_CLOSE, kernel);

        // Remove Small Blobs
        cv::Mat opening;
        cv::morphologyEx(closing, opening, cv::MORPH_OPEN, kernel);

        // Merge adjacent big blobs
        cv::dilate(opening, dst, kernel, cv::Point(-1,-1), 2);
    } else {
        cv::Mat operationsRst = src.clone();
        cv::dilate(operationsRst, operationsRst, kernel);
        cv::dilate(operationsRst, operationsRst, kernel);
        cv::erode(operationsRst, operationsRst, kernel);
        cv::dilate(operationsRst, operationsRst, kernel);
        cv::dilate(operationsRst, operationsRst, kernel);
        cv::erode(operationsRst, operationsRst, kernel);
        dst = operationsRst.clone();
    }
}

void BGMOGDetector::_filterParamMorphSettingHandler(int value, void *data)
{
    int *morph = static_cast<int *>(data);
    *morph = std::max(*morph, 1);
}
