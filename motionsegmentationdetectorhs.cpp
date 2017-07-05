#include "motionsegmentationdetectorhs.h"

#include "trafficsurveillancecommon.h"
#include "trafficdebug.h"

#include <opencv2/legacy/legacy.hpp>

MotionSegmentationDetectorHS::MotionSegmentationDetectorHS(double alpha, int morphBoxSize) :
    _alpha(alpha),
    _morphBoxSize(morphBoxSize),
    _startFlag(true),
    _showFlowFrameFlag(true),
    _showMagFlowFlag(false),
    _showBinaryMagFlowFlag(false),
    _showMorphFlag(true),
    _flowWinName("Optical Flow"),
    _filteredFlowWinName("Filtered Flow"),
    _magFlowWinName("Flow Magniude"),
    _binaryMagFlowWinName("Binarized Flow Magnitude"),
    _morphWinName("Morphed Filtered Flow")
{
    _alphaChangeable = static_cast<int>(100 * _alpha);
    _maxAlpha = 7500;
}

void MotionSegmentationDetectorHS::process(const cv::Mat &frame, cv::Mat &result)
{
    if (_startFlag) {
        result = cv::Mat::zeros(frame.size(), CV_8UC1);
        _prevFrame = frame.clone();
        cv::cvtColor(_prevFrame, _prevFrame, CV_BGR2GRAY);

        if (_showFlowFrameFlag) {
            setWindow(_flowWinName);
            setWindow(_filteredFlowWinName);
        }

        if (_showMagFlowFlag) {
            setWindow(_magFlowWinName);
        }

        if (_showBinaryMagFlowFlag) {
            setWindow(_binaryMagFlowWinName);
        }

        if (_showMorphFlag) {
            setWindow(_morphWinName);
            cv::createTrackbar("HS Alpha (/100)", _morphWinName, &_alphaChangeable, _maxAlpha,
                               _filterParamAlphaSettingHandler, static_cast<void *>(&_alpha));
            cv::createTrackbar("Morph Size", _morphWinName, &_morphBoxSize, 100,
                               _filterParamMorphSettingHandler, static_cast<void *>(&_morphBoxSize));
        }

        _startFlag = false;
    } else {
        cv::Mat currFrame = frame.clone();
        cv::cvtColor(currFrame, currFrame, CV_BGR2GRAY);

        // Calculate Optical Flow
        cv::Mat flow;
        _calculateFlowHS(currFrame, _prevFrame, flow, _alpha);

        // Filter Optical Flow
        cv::Mat filteredFlow;
        _filterFlow(flow, filteredFlow, false);
        if (_showFlowFrameFlag) {
            _drawOpticalFlowImage(_flowImage, flow, frame, 10);
            _drawOpticalFlowImage(_filteredFlowImage, filteredFlow, frame, 10, CV_RGB(0, 0, 255));
            cv::imshow(_flowWinName, _flowImage);
            cv::imshow(_filteredFlowWinName, _filteredFlowImage);
        }

        // Normalize Magnitude
        cv::Mat filteredFlowX, filteredFlowY, flowMag, flowMag8b;
        _splitFlows(filteredFlow, filteredFlowX, filteredFlowY);
        cv::magnitude(filteredFlowX, filteredFlowY, flowMag);
        cv::normalize(flowMag, flowMag8b, 0, 255, cv::NORM_MINMAX, CV_8U);
        if (_showMagFlowFlag) {
            cv::imshow(_magFlowWinName, flowMag8b);
        }

        // Binarize Mag Flow
        cv::Mat binaryFlow;
        cv::threshold(flowMag8b, binaryFlow, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
        if (_showBinaryMagFlowFlag) {
            cv::imshow(_binaryMagFlowWinName, binaryFlow);
        }

        // Morphological Filter
        _morphFilter(binaryFlow, result, cv::MORPH_OPEN, _morphBoxSize);
        if (_morphBoxSize) {
            cv::imshow(_morphWinName, result);
        }

        // Update
        _prevFrame = currFrame.clone();
    }
}

void MotionSegmentationDetectorHS::init()
{
}

void MotionSegmentationDetectorHS::_calculateFlowHS(const cv::Mat &current, const cv::Mat &previous, cv::Mat &flow,
                                                    double alpha, int iterations)
{
    static CvMat *flowX = cvCreateMat(current.rows, current.cols, CV_32FC1);
    static CvMat *flowY = cvCreateMat(current.rows, current.cols, CV_32FC1);
    static CvMat *prev = cvCreateMat(current.rows, current.cols, CV_8UC1);
    static CvMat *next = cvCreateMat(current.rows, current.cols, CV_8UC1);
    static CvTermCriteria endCriteria = {CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, iterations, 1e-10};

    previous.copyTo(cv::Mat(prev));
    current.copyTo(cv::Mat(next));

    cvCalcOpticalFlowHS(prev, next, false, flowX, flowY, alpha, endCriteria);

    _mergeFlows(cv::Mat(flowX), cv::Mat(flowY), flow);

}

void MotionSegmentationDetectorHS::_filterFlow(const cv::Mat &flows, cv::Mat &filteredFlows, bool adaptative = true)
{
    cv::Mat flowX, flowY;
    _splitFlows(flows, flowX, flowY);

    cv::Mat filtFlowX, filtFlowY;
    if (adaptative) {
        cv::Mat filtFlowXu, filtFlowYu;
        cv::Mat flowXu, flowYu;
        double minX, minY, maxX, maxY;
        cv::minMaxIdx(flowX, &minX, &maxX);
        cv::minMaxIdx(flowX, &minY, &maxY);
        flowX.convertTo(flowXu, CV_8UC1, 255.0 / (maxX - minX));
        flowY.convertTo(flowYu, CV_8UC1, 255.0 / (maxY - minY));
        cv::adaptiveBilateralFilter(flowXu, filtFlowXu, cv::Size(11, 11), 50);
        cv::adaptiveBilateralFilter(flowYu, filtFlowYu, cv::Size(11, 11), 50);

        filtFlowXu.convertTo(filtFlowX, CV_32FC1, (maxX - minX) / 255.0);
        filtFlowYu.convertTo(filtFlowY, CV_32FC1, (maxX - minX) / 255.0);
    } else {
        int __bDist__ = 10;
        cv::bilateralFilter(flowX, filtFlowX, __bDist__, 2 * __bDist__, __bDist__ / 2);
        cv::bilateralFilter(flowY, filtFlowY, __bDist__, 2 * __bDist__, __bDist__ / 2);
    }
    _mergeFlows(filtFlowX, filtFlowY, filteredFlows);
}

void MotionSegmentationDetectorHS::_mergeFlows(const cv::Mat &flowX, const cv::Mat &flowY, cv::Mat &flowXY)
{
    // Merge Flows Together
    std::vector<cv::Mat> flows;
    flows.push_back(flowX);
    flows.push_back(flowY);
    cv::merge(flows, flowXY);
}

void MotionSegmentationDetectorHS::_splitFlows(const cv::Mat &flowXY, cv::Mat &flowX, cv::Mat &flowY)
{
    // Split Flows
    std::vector<cv::Mat> flows;
    cv::split(flowXY, flows);
    flowX = flows.at(0);
    flowY = flows.at(1);
}

void MotionSegmentationDetectorHS::_morphFilter(const cv::Mat &src, cv::Mat &dst, int operation, int structElementSize)
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

void MotionSegmentationDetectorHS::_filterParamMorphSettingHandler(__attribute__((unused)) int value, void *data)
{
    int *morph = static_cast<int *>(data);
    *morph = *morph < 1 ? 1 : *morph;
}

void MotionSegmentationDetectorHS::_filterParamAlphaSettingHandler(int value, void *data)
{
    double *alpha = static_cast<double *>(data);
    *alpha = value / 100.0;
    *alpha = *alpha < 0.1 ? 0.1 : *alpha;
}
