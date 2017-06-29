#ifndef FLOWDRAWER_H
#define FLOWDRAWER_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/legacy/legacy.hpp>

class FlowDrawer
{
public:
    void _drawOpticalFlowImage (cv::Mat& flowImage, const cv::Mat& flowXY, const cv::Mat &original, int step,
                               const cv::Scalar& color = CV_RGB(255, 0, 0));
};

#endif // FLOWDRAWER_H
