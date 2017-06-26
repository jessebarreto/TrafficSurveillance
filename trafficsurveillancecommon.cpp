#include "trafficsurveillancecommon.h"

void methodProject7Req1(cv::VideoCapture &video, const cv::Mat &curFrame, double alpha, int morphSize)
{

}

void methodProject7Req2(cv::VideoCapture &video, const cv::Mat &curFrame, int type)
{

}

void setWindow(const std::string &winName)
{
    cv::namedWindow(winName, CV_WINDOW_NORMAL);
    cv::resizeWindow(winName, 640, 480);
}