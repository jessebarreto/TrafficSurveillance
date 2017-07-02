#include "car.h"

#include "trafficsurveillancecommon.h"

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
    _matched(false),
    _boundRect(boundRect)
{
    _positions.push_back(initialPosition);
    double angle;
    getVectorPolarCoord(_boundRect.tl(), _boundRect.br(), &_carDiagonal, &angle);
}

cv::Point &Car::getLastPosition()
{
    return _positions.back();
}

cv::Point &Car::getFirstPosition()
{
    return _positions.front();
}

int Car::getPositionsSize()
{
    return static_cast<int>(_positions.size());
}

void Car::updatePosition(const cv::Point &newPosition)
{
    _positions.push_back(newPosition);
    _boundRect.x = newPosition.x - _boundRect.width / 2;
    _boundRect.y = newPosition.y - _boundRect.height / 2;
    _lastSeenFrame = 0;
}

void Car::update(const cv::Point &newPosition, const cv::Rect boundRect)
{
    updatePosition(newPosition);
    _boundRect = boundRect;
}

void Car::drawCar(cv::Mat &image)
{
    cv::Scalar color = carColours.at(_carNumber % carColours.size());
    for (cv::Point &point : _positions) {
        cv::circle(image, point, 2, color, -1);
        cv::polylines(image, _positions, false, color);
        cv::rectangle(image, _boundRect, color);
        cv::putText(image, std::to_string(_carNumber), _boundRect.tl(), cv:: FONT_HERSHEY_SIMPLEX, 0.5,
                    cv::Scalar::all(0));
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

void Car::_predictNextPosition()
{
    int numPositions = static_cast<int>(_positions.size());

    if (numPositions == 1) {
        _predictedPos.x = _positions.back().x;
        _predictedPos.y = _positions.back().y;
    } else if (numPositions == 2) {
        int deltaX = _positions[1].x - _positions[0].x;
        int deltaY = _positions[1].y - _positions[0].y;

        _predictedPos.x = _positions.back().x + deltaX;
        _predictedPos.y = _positions.back().y + deltaY;
    }
    else if (numPositions == 3) {
        int sumOfXChanges = ((_positions[2].x - _positions[1].x) * 2) + ((_positions[1].x - _positions[0].x) * 1);
        int sumOfYChanges = ((_positions[2].y - _positions[1].y) * 2) + ((_positions[1].y - _positions[0].y) * 1);
        int deltaX = (int)std::round((float)sumOfXChanges / 3.0);
        int deltaY = (int)std::round((float)sumOfYChanges / 3.0);

        _predictedPos.x = _positions.back().x + deltaX;
        _predictedPos.y = _positions.back().y + deltaY;
    } else if (numPositions == 4) {
        int sumOfXChanges = ((_positions[3].x - _positions[2].x) * 3) + ((_positions[2].x - _positions[1].x) * 2) +
                ((_positions[1].x - _positions[0].x) * 1);
        int sumOfYChanges = ((_positions[3].y - _positions[2].y) * 3) + ((_positions[2].y - _positions[1].y) * 2) +
                ((_positions[1].y - _positions[0].y) * 1);
        int deltaX = (int)std::round((float)sumOfXChanges / 6.0);
        int deltaY = (int)std::round((float)sumOfYChanges / 6.0);

        _predictedPos.x = _positions.back().x + deltaX;
        _predictedPos.y = _positions.back().y + deltaY;
    }
    else {//if (numPositions >= 5)
        int sumOfXChanges = ((_positions[numPositions - 1].x - _positions[numPositions - 2].x) * 4) +
            ((_positions[numPositions - 2].x - _positions[numPositions - 3].x) * 3) +
            ((_positions[numPositions - 3].x - _positions[numPositions - 4].x) * 2) +
            ((_positions[numPositions - 4].x - _positions[numPositions - 5].x) * 1);
        int sumOfYChanges = ((_positions[numPositions - 1].y - _positions[numPositions - 2].y) * 4) +
            ((_positions[numPositions - 2].y - _positions[numPositions - 3].y) * 3) +
            ((_positions[numPositions - 3].y - _positions[numPositions - 4].y) * 2) +
            ((_positions[numPositions - 4].y - _positions[numPositions - 5].y) * 1);
        int deltaX = (int)std::round((float)sumOfXChanges / 10.0);
        int deltaY = (int)std::round((float)sumOfYChanges / 10.0);

        _predictedPos.x = _positions.back().x + deltaX;
        _predictedPos.y = _positions.back().y + deltaY;
    }
}

cv::Point &Car::getPredictedPosition()
{
    _predictNextPosition();
    return _predictedPos;
}

double Car::getCarDiagonalSize()
{
    return _carDiagonal;
}

void Car::setMatched(bool matchFlag)
{
    _matched = matchFlag;
}

bool Car::getMatched()
{
    return _matched;
}
