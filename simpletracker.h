#ifndef SIMPLETRACKER_H
#define SIMPLETRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include <vector>

#include "virtualtrack.h"
#include "car.h"

class SimpleTracker : public VirtualTrack
{
    // Algorithm Parameters
    cv::Size _minCarSize;
    cv::Size _maxCarSize;
    int _maxUnseenFrames;
    int _maxSpeed;

    // Car Countours
    std::vector<std::vector<cv::Point>> _carContours;
    std::vector<std::vector<cv::Point>> _carHulls;
    cv::Mat _imgHull;
    int _carCounter;

    bool _startFlag;
    cv::Scalar _boundRectColor;

    bool _validateSceneCar(const cv::Rect& boundRect);

    cv::Point _getCarCentroid(const cv::Rect& boundRect);

    void _updateCarsV2(std::vector<Car *> &cars, std::vector<std::pair<cv::Point , cv::Rect>> &sceneCars, ImageLine &line);

    void _updateCars(std::vector<Car *> &cars, std::vector<std::pair<cv::Point , cv::Rect>> &sceneCars, ImageLine &line);

    bool _updateCar(Car &car, std::vector<std::pair<cv::Point, cv::Rect> > &sceneCars, ImageLine &line);

    bool _isValidSpeed(double magnitude, double angle, double maxSpeed, double minSpeed, bool horizontal);
public:
    SimpleTracker(const cv::Size &minCarSize, const cv::Size &maxCarSize,
                        int maxUnseenFrames, int maxSpeed);

    int process(const cv::Mat &frame, const cv::Mat &srcBlobs, ImageLine &imageLine,
                std::vector<Car *> &cars) override;
};

#endif // SIMPLETRACKER_H
