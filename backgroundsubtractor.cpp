#include "backgroundsubtractor.h"

void BackgroundSubtractor::_morphFilter(const cv::Mat &src, cv::Mat &dst, int operation, int structElementSize)
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(structElementSize, structElementSize));

    if (operation == cv::MORPH_OPEN) {
        // Remove Small Blobs
        cv::Mat opening;
        cv::morphologyEx(src, opening, cv::MORPH_OPEN, kernel);

        // Fill holes
        cv::Mat closing;
        cv::morphologyEx(opening, closing, cv::MORPH_CLOSE, kernel);

        // Merge adjacent big blobs
        cv::dilate(closing, dst, kernel, cv::Point(-1,-1), 2);
    } else {
        // Fill holes
        cv::Mat closing;
        cv::morphologyEx(src, closing, cv::MORPH_CLOSE, kernel);

        // Remove Small Blobs
        cv::Mat opening;
        cv::morphologyEx(closing, opening, cv::MORPH_OPEN, kernel);

        // Merge adjacent big blobs
        cv::dilate(opening, dst, kernel, cv::Point(-1,-1), 2);
    }
}

BackgroundSubtractor::BackgroundSubtractor(IBGS *bgRef) :
    _bgs(bgRef)
{
    _thresholdValue = 150;
}

BackgroundSubtractor::~BackgroundSubtractor()
{
    if (_bgs) {
        delete _bgs;
    }
}

void BackgroundSubtractor::process(const cv::Mat &frame, cv::Mat &result)
{
    cv::Mat partialResult;
    _bgs->process(frame, partialResult, _imgBgkModel);

    // Apply Threshold
    cv::Mat binarizedFGMask;
    cv::threshold(partialResult, binarizedFGMask, _thresholdValue, 255, cv::THRESH_BINARY);

    // Apply Morph filter
    _morphFilter(binarizedFGMask, result, cv::MORPH_OPEN, _morphBoxSize);
}

void BackgroundSubtractor::init()
{
}

void BackgroundSubtractor::setMorphSize(int morphSize)
{
    _morphBoxSize = morphSize;
}
