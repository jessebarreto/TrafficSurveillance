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
    bool _matched;
    double _carDiagonal;
    cv::Point _predictedPos;
    cv::Rect _boundRect;

    static const std::vector<cv::Scalar> carColours;
protected:
    void _predictNextPosition();

public:
    Car(int number, const cv::Point &initialPosition, const cv::Rect &boundRect);

    cv::Point &getLastPosition();

    cv::Point &getFirstPosition();

    int getPositionsSize();

    void updatePosition(const cv::Point &newPosition);

    void update(const cv::Point &newPosition, const cv::Rect boundRect);

    void drawCar(cv::Mat &image);

    void notFounded();

    void countCar(bool count);

    bool wasCounted();

    int getLastSeenFrame();

    cv::Point &getPredictedPosition();

    double getCarDiagonalSize();

    void setMatched(bool matchFlag);

    bool getMatched();
};

#endif // CAR_H
