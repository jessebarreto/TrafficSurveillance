#ifndef CAR_H
#define CAR_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

class Car
{
    // Cars Caracteristics in the scene
    int _carNumber;
    std::vector<cv::Point> _positions;
    int _lastSeenFrame;
    bool _countedFlag;
    cv::Rect _boundRect;

    static const std::vector<cv::Scalar> carColours;
public:
    Car(int number, const cv::Point &initialPosition, const cv::Rect &boundRect);

    cv::Point &getLastPosition();

    void updatePosition(const cv::Point &newPosition);

    void drawCar(cv::Mat &image);

    void notFounded();

    void countCar(bool count);

    bool wasCounted();

    int getLastSeenFrame();
};

#endif // CAR_H
