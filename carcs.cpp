#include "carcs.h"

#include <vector>

CarCS::CarCS(int vmin, int vmax, int smin, int number, const cv::Point &initialPosition, const cv::Rect &boundRect, const cv::Mat &frame) :
    Car(number, initialPosition, boundRect)
{
    // Initialization
    _trackedWindow = cv::Rect(initialPosition, boundRect.size());

    _roi = frame(_trackedWindow);
    cv::cvtColor(_roi, _hsvRoi, CV_BGR2HSV);
    cv::inRange(_hsvRoi, cv::Scalar(0, smin, MIN(_vmin, _vmax)), cv::Scalar(180, 256, MAX(_vmin, _vmax)), _mask);
    _mask.convertTo(_mask, CV_8UC1);
    const int ch[] = {0, 0};
    float hranges[] = {0,180};
    const float* phranges = hranges;
    int hsize = 16;

    cv::calcHist(&_hsvRoi, 1, 0, _mask, _roiHist, 1, &hsize, &phranges);
    cv::normalize(_roiHist, _roiHist, 0, 255, cv::NORM_MINMAX);

    _criteria = cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1);
}

void CarCS::update(const cv::Mat &frame, const cv::Point &newPosition, const cv::Rect boundRect)
{
    float hranges[] = {0,180};
    int channel[] = {0};
    const float* phranges = hranges;
    const int *ch = channel;

    cv::Mat hsv;
    cv::Mat backProj = cv::Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    cv::cvtColor(frame, hsv, CV_BGR2HSV);

    cv::calcBackProject(&hsv, 1, 0, _roiHist, backProj, &phranges);

    cv::CamShift(backProj, _trackedWindow, _criteria);

    Car::update(_trackedWindow.tl(), boundRect);
}

void CarCS::notFounded()
{
    Car::notFounded();
}

cv::Point &CarCS::getPredictedPosition()
{

}

void CarCS::_predictNextPositionCS()
{

}


