#ifndef VIRTUALDETECTION_H
#define VIRTUALDETECTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

/*!
 * \brief The VirtualMethod class is implemented by the other possible methods.
 */
class VirtualDetection
{
public:
    // Process Identification Method
    virtual void process(const cv::Mat &frame, cv::Mat &result) = 0;

    // Initialization
    virtual void init() = 0;

    // Set Morph Size
    virtual void setMorphSize(int morphSize) = 0;
};

#endif // VIRTUALDETECTION_H
