#ifndef VIRTUALTRACK_H
#define VIRTUALTRACK_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "imageline.h"
#include "car.h"

/*!
 * \brief The VirtualTrack class is implemented by other possible trackers.
 */
class VirtualTrack
{
public:
    /*!
     * \brief Track cars in the scene
     */
    virtual int process(const cv::Mat &frame, const cv::Mat &srcBlobs, ImageLine &imageLine,
                         std::vector<Car> &cars) = 0;
};

#endif // VIRTUALTRACK_H
