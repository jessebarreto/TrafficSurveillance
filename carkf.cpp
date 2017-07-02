#include "carkf.h"

CarKF::CarKF(int number, const cv::Point &initialPosition,
             const cv::Rect &boundRect) :
    Car(number, initialPosition, boundRect),
    _stateSize(4),
    _measureSize(2),
    _filter(_stateSize, _measureSize),
    _states(_stateSize, 1, CV_32FC1), // [x,y,v_x,v_y]
    _measures(_measureSize, 1, CV_32FC1) // [z_x,z_y,z_w,z_h]
{
    _time = 0.0;

    cv::setIdentity(_filter.transitionMatrix);
    _filter.measurementMatrix = cv::Mat::zeros(_measureSize, _stateSize, CV_32F);
    _filter.measurementMatrix.at<float>(0) = 1.0f;
    _filter.measurementMatrix.at<float>(5) = 1.0f;
    cv::setIdentity(_filter.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(_filter.errorCovPost, cv::Scalar::all(1));

    _measures.at<float>(0) = initialPosition.x;
    _measures.at<float>(1) = initialPosition.y;

    // Initialization
    _filter.errorCovPre.at<float>(0) = 1; // px
    _filter.errorCovPre.at<float>(5) = 1; // px
    _filter.errorCovPre.at<float>(10) = 1;
    _filter.errorCovPre.at<float>(15) = 1;

    _states.at<float>(0) = _measures.at<float>(0);
    _states.at<float>(1) = _measures.at<float>(1);
    _states.at<float>(2) = 0;
    _states.at<float>(3) = 0;

    _filter.statePost = _states;
}

void CarKF::update(const cv::Point &newPosition, const cv::Rect boundRect)
{
    _measures.at<float>(0) = newPosition.x;
    _measures.at<float>(1) = newPosition.y;

    _filter.correct(_measures);

    Car::update(newPosition, boundRect);
}

void CarKF::updateFilterTime()
{
    double prevTime = _time;
    _time = (double)cv::getTickCount();
    _dT = (_time - prevTime) / cv::getTickFrequency();
}

void CarKF::notFounded()
{
    Car::notFounded();
}

cv::Point &CarKF::getPredictedPosition()
{
    _predictNextPositionKF();
    return _nextPos;
}

void CarKF::_predictNextPositionKF()
{
    _states = _filter.predict();
    _nextPos.x = static_cast<int>(_states.at<float>(0));
    _nextPos.y = static_cast<int>(_states.at<float>(5));
}


