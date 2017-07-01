#ifndef KALMANFILTERTRACKER_H
#define KALMANFILTERTRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include <vector>

#include "virtualtrack.h"
#include "car.h"

class KalmanFilterTracker : public VirtualTrack
{
    // Algorithm Parameters
    cv::Size _minCarSize;
    cv::Size _maxCarSize;
    int _maxUnseenFrames;

    // Car Countours
    std::vector<std::vector<cv::Point>> _carContours;
    int _carCounter;

    bool _startFlag;
    cv::Scalar _boundRectColor;

    bool _showAllCountours;
    std::string _allCountoursWinName;

    bool _validateCar(const cv::Rect& boundRect);

    cv::Point _getCarCentroid(const cv::Rect& boundRect);

    bool _updateCar(Car &car, std::vector<std::pair<cv::Point , cv::Rect>> &sceneCars);

    bool _isValidSpeed(double magnitude, double maxSpeed, double minSpeed);
public:
    KalmanFilterTracker(const cv::Size &minCarSize, const cv::Size &maxCarSize, int maxUnseenFrames);

    void process(const cv::Mat &frame, const cv::Mat &srcBlobs, const ImageLine &imageLine,
                 std::vector<Car> &cars) override;
};

#endif // KALMANFILTERTRACKER_H
