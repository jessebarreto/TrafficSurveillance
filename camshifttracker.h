#ifndef CAMSHIFTTRACKER_H
#define CAMSHIFTTRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include <vector>

#include "virtualtrack.h"
#include "car.h"

class CamShiftTracker : public VirtualTrack
{
    // Algorithm Parameters
    int _vmin, _vmax, _smin;
    int _maxUnseenFrames;

    // Car Contours
    std::vector<std::vector<cv::Point>> _carContours;
    int _carCounter;

    bool _startFlag;

    bool _validateCar(const cv::Rect& boundRect);

    cv::Point _getCarCentroid(const cv::Rect& boundRect);

    void _updateCars(const cv::Mat &frame, std::vector<Car *> &cars, std::vector<std::pair<cv::Point, cv::Rect> > &sceneCars, ImageLine &line);

public:
    CamShiftTracker(int vmin, int vmax, int smin, const cv::Size &minCarSize, const cv::Size &maxCarSize,
                        int maxUnseenFrames);

    int process(const cv::Mat &frame, const cv::Mat &srcBlobs, ImageLine &imageLine,
                 std::vector<Car *> &cars) override;
};

#endif // CAMSHIFTTRACKER_H
