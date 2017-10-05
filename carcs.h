#ifndef CARCS_H
#define CARCS_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "car.h"

class CarCS : public Car
{
    int _vmin, _vmax, _smin;

    cv::Point _nextPos;
    cv::Rect _trackedWindow;
    cv::Mat _roi;
    cv::Mat _hsvRoi;
    cv::Mat _mask;
    cv::Mat _roiHist;
    cv::TermCriteria _criteria;

    void _predictNextPositionCS();

public:
    CarCS(int vmin, int vmax, int smin, int number, const cv::Point &initialPosition, const cv::Rect &boundRect, const cv::Mat &frame);

    void update(const cv::Mat &frame, const cv::Point &newPosition, const cv::Rect boundRect);

    void notFounded();

    cv::Point &getPredictedPosition();
};

#endif // CARCS_H
