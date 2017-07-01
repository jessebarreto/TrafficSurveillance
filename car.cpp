#include "car.h"

const std::vector<cv::Scalar> Car::carColours({CV_RGB(0, 0, 255),
              CV_RGB(0, 255, 0),
              CV_RGB(255, 0 ,0),
              CV_RGB(121, 240, 154),
              CV_RGB(0, 106, 255),
              CV_RGB(255, 0, 178),
              CV_RGB(255,148,0),
              CV_RGB(144,55,255)});

Car::Car(int number, const cv::Point &initialPosition, const cv::Rect &boundRect) :
    _carNumber(number),
    _lastSeenFrame(0),
    _countedFlag(false),
    _boundRect(boundRect)
{
    _positions.push_back(initialPosition);
}

cv::Point &Car::getLastPosition()
{
    return _positions.back();
}

void Car::updatePosition(const cv::Point &newPosition)
{
    _positions.push_back(newPosition);
    _boundRect.x = newPosition.x - _boundRect.width / 2;
    _boundRect.y = newPosition.y - _boundRect.height / 2;
    _lastSeenFrame = 0;
}

void Car::drawCar(cv::Mat &image)
{
    cv::Scalar color = carColours.at(_carNumber % carColours.size());
    for (cv::Point &point : _positions) {
        cv::circle(image, point, 2, color, -1);
        cv::polylines(image, _positions, false, color);
        cv::rectangle(image, _boundRect, color);
    }
}

void Car::notFounded()
{
    _lastSeenFrame++;
}

void Car::countCar(bool count)
{
    _countedFlag = count;
}

bool Car::wasCounted()
{
    return _countedFlag;
}

int Car::getLastSeenFrame()
{
    return _lastSeenFrame;
}
