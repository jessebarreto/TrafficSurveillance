#ifndef IMAGELINE_H
#define IMAGELINE_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

class ImageLine
{
    std::pair<cv::Point *, cv::Point *> _points;

public:
    // Constructors
    ImageLine();
    ImageLine(cv::Point *lineBegin, cv::Point *lineEnd);

    // Destructor
    ~ImageLine();

    // Clear Points
    void clearBeginPoint();
    void clearEndPoint();

    // Line Begin
    void setLineBegin(cv::Point *lineBegin);
    cv::Point &getLineBegin();

    // Line End
    void setLineEnd(cv::Point *lineEnd);
    cv::Point &getLineEnd();

    // When Line is Complete
    bool isCompleted();

    // Draw itself in a colorful image
    void draw(cv::Mat &image);
};

#endif // IMAGELINE_H
