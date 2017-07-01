#include "kalmanfiltertracker.h"

#include "trafficsurveillancecommon.h"

KalmanFilterTracker::KalmanFilterTracker(const cv::Size &minCarSize, const cv::Size &maxCarSize, int maxUnseenFrames) :
    _minCarSize(minCarSize),
    _maxCarSize(maxCarSize),
    _maxUnseenFrames(maxUnseenFrames),
    _carCounter(0),
    _startFlag(true),
    _boundRectColor(CV_RGB(0,0,255)),
    _showAllCountours(false),
    _allCountoursWinName("All Countours")
{

}

void KalmanFilterTracker::process(const cv::Mat &frame, const cv::Mat &srcBlobs, const ImageLine &imageLine, std::vector<Car> &cars)
{
    int i;
    if (_startFlag) {
        setWindow(_allCountoursWinName);

        _startFlag = false;
    } else {
        // Find Contours
        cv::findContours(srcBlobs, _carContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        if (_showAllCountours) {
            cv::Mat frameWContours = frame.clone();
            cv::drawContours(frameWContours, _carContours, -1, CV_RGB(255, 0, 0));
            cv::imshow(_allCountoursWinName, frameWContours);
        }

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
        i = 0;
        for (Car &car : cars) {
            bool updated = _updateCar(car, sceneCars);
            if (updated) {
                sceneCars.erase(sceneCars.begin() + i);
            }
            i++;
        }

        // Create New Cars with the new sceneCars
        for (std::pair<cv::Point , cv::Rect> &sceneCar : sceneCars) {
            cars.push_back(Car(cars.size(), sceneCar.first, sceneCar.second));
        }

        // Count Cars that crosses the line
        for (Car &car : cars) {
            if (!car.wasCounted()) {
                _carCounter++;
                car.countCar(true);
            }
        }

        // Remove cars which did not appear for a while
//        i = 0;
//        for (Car &car : cars) {
//            if (car.getLastSeenFrame() >= _maxUnseenFrames) {
//                cars.erase(cars.begin() + i);
//            }
//            i++;
//        }
    }
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

bool KalmanFilterTracker::_updateCar(Car &car, std::vector<std::pair<cv::Point, cv::Rect> > &sceneCars)
{
    // Search for matches
    for (std::pair<cv::Point , cv::Rect> &sceneCar : sceneCars) {
        double magnitude, angle;
        getVectorPolarCoord(car.getLastPosition(), sceneCar.first, &magnitude, &angle);
        if (_isValidSpeed(magnitude, 10, 2)) {
            car.updatePosition(sceneCar.first);
            return true;
        }
    }

    // No Matching
    car.notFounded();
    return false;
}

bool KalmanFilterTracker::_isValidSpeed(double magnitude, double maxSpeed, double minSpeed)
{
//    double thresholdMax = std::max(maxSpeed, angle*angle * -26.2624508001 + 22.9183118052 * angle + 25.0); // MAGIC EQUATION
//    return (magnitude <= thresholdMax && magnitude >= minSpeed);
    return (magnitude <= maxSpeed && magnitude > minSpeed);
}
