#ifndef CARKF_H
#define CARKF_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "car.h"

class CarKF : public Car
{
    int _stateSize;
    int _measureSize;

    cv::KalmanFilter _filter;

    cv::Mat _states;
    cv::Mat _measures;

    double _time, _dT;

    void _predictNextPositionKF();
    cv::Point _nextPos;

public:
    CarKF(int number, const cv::Point &initialPosition, const cv::Rect &boundRect);

    void update(const cv::Point &newPosition, const cv::Rect boundRect);

    void updateFilterTime();

    void notFounded();

    cv::Point &getPredictedPosition();
};

#endif // CARKF_H
