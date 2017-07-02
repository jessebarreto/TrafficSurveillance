#include "kalmanfiltertracker.h"

#include "trafficsurveillancecommon.h"

#include "carkf.h"

KalmanFilterTracker::KalmanFilterTracker(const cv::Size &minCarSize, const cv::Size &maxCarSize, int maxUnseenFrames) :
    _minCarSize(minCarSize),
    _maxCarSize(maxCarSize),
    _maxUnseenFrames(maxUnseenFrames),
    _carCounter(0),
    _startFlag(true)
{
}

int KalmanFilterTracker::process(const cv::Mat &frame, const cv::Mat &srcBlobs, ImageLine &imageLine,
                                  std::vector<Car *> &cars)
{
    // Find Contours
    cv::findContours(srcBlobs, _carContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Find Contours that fits a car
    std::vector<std::pair<cv::Point , cv::Rect>> sceneCars;
    for (size_t i = 0; i < _carContours.size(); i++) {
        cv::Rect boundRect = cv::boundingRect(_carContours.at(i));
        if (!_validateCar(boundRect)) {
            continue;
        }
        sceneCars.push_back(std::make_pair(_getCarCentroid(boundRect), boundRect));
    }

    // Update the Existing Vehicles
    _updateCars(cars, sceneCars, imageLine);

    // Create New Cars with the new sceneCars
    for (std::pair<cv::Point , cv::Rect> &sceneCar : sceneCars) {
        cars.push_back(new CarKF(cars.size(), sceneCar.first, sceneCar.second));
    }

    // Count Cars that crosses the line
    for (Car *car : cars) {
        if (!car->wasCounted() && carCrossesLine(*car, imageLine)) {
            _carCounter++;
            car->countCar(true);
        }
    }

    // Remove cars which did not appear for a while
    for (size_t id = 0; id < cars.size(); id++) {
        if (cars.at(id)->getLastSeenFrame() >= _maxUnseenFrames) {
            delete cars[id];
            cars.erase(cars.begin() + id);
        }
    }

    _startFlag = false;
    return _carCounter;
}

bool KalmanFilterTracker::_validateCar(const cv::Rect &boundRect)
{
    return (boundRect.width >= _minCarSize.width && boundRect.height >= _minCarSize.height &&
            boundRect.width < _maxCarSize.width && boundRect.height < _maxCarSize.height);
}

cv::Point KalmanFilterTracker::_getCarCentroid(const cv::Rect &boundRect)
{
    int cx = boundRect.x + boundRect.width / 2;
    int cy = boundRect.y + boundRect.height / 2;

    return cv::Point(cx, cy);
}

void KalmanFilterTracker::_updateCars(std::vector<Car *> &cars, std::vector<std::pair<cv::Point, cv::Rect> > &sceneCars, ImageLine &line)
{
    if (!cars.empty()) {
        for (Car *car : cars) {
            car->setMatched(false);
            CarKF *carkf = static_cast<CarKF *>(car);
            carkf->updateFilterTime();
        }

        int idFrame = 0;
        for (std::pair<cv::Point , cv::Rect> &sceneCar : sceneCars) {
            double leastDistance = std::numeric_limits<double>::max();
            int indexLeastDistance = 0;
            for (size_t id = 0; id < cars.size(); id++) {
                double magnitude, angle;
                getVectorPolarCoord(sceneCar.first, cars[id]->getPredictedPosition(), &magnitude, &angle);
                if (magnitude < leastDistance) {
                    leastDistance = magnitude;
                    indexLeastDistance = id;
                }
            }

            if (leastDistance < cars[indexLeastDistance]->getCarDiagonalSize()) {
                cars[indexLeastDistance]->update(sceneCar.first, sceneCar.second);
                sceneCars.erase(sceneCars.begin() + idFrame);
                cars[indexLeastDistance]->setMatched(true);
            }
            idFrame++;
        }

        for (Car *car : cars) {
            if (!car->getMatched()) {
                car->notFounded();
            }
        }
    }
}
