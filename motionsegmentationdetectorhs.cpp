#include "motionsegmentationdetectorhs.h"

#include "trafficsurveillancecommon.h"

#include <opencv2/legacy/legacy.hpp>

MotionSegmentationDetectorHS::MotionSegmentationDetectorHS(double alpha, int morphBoxSize) :
    _alpha(alpha),
    _morphBoxSize(morphBoxSize),
    _startFlag(true),
    _showFlowFrameFlag(true)
{
}

void MotionSegmentationDetectorHS::process(const cv::Mat &frame, cv::Mat &result)
{
    if (_startFlag) {
        result = cv::Mat::zeros(frame.size(), CV_8UC1);
        _prevFrame = frame.clone();
        cv::cvtColor(_prevFrame, _prevFrame, CV_BGR2GRAY);

        if (_showFlowFrameFlag) {
            _flowWinName = "Optical Flow";
            setWindow(_flowWinName);
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
        _filterFlow(flow, filteredFlow);

        if (_showFlowFrameFlag) {
            _drawOpticalFlowImage(_flowImage, filteredFlow, frame, 10);
            cv::imshow(_flowWinName, _flowImage);
        }



        // Update
        _prevFrame = currFrame.clone();
    }
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

void MotionSegmentationDetectorHS::_filterFlow(const cv::Mat &flows, cv::Mat &filteredFlows)
{
    cv::Mat flowX, flowY;
    _splitFlows(flows, flowX, flowY);

    flowX.convertTo(flowX, CV_8UC1);
    flowY.convertTo(flowY, CV_8UC1);
    cv::fastNlMeansDenoising(flowX, flowX);
    cv::fastNlMeansDenoising(flowY, flowY);

    flowX.convertTo(flowX, CV_32FC1);
    flowY.convertTo(flowY, CV_32FC1);
    _mergeFlows(flowX, flowY, filteredFlows);
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
    flowX = flows.at(1);
}
