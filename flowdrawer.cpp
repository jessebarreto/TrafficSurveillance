#include "flowdrawer.h"

void FlowDrawer::_drawOpticalFlowImage(cv::Mat &flowImage, const cv::Mat &flowXY, const cv::Mat &original, int step, const cv::Scalar &color)
{
    flowImage = original.clone();
    for(int y = 0; y < flowXY.rows; y += step) {
        for(int x = 0; x < flowXY.cols; x += step)
        {
            const cv::Point2f& fxy = flowXY.at< cv::Point2f>(y, x);
            cv::line(flowImage, cv::Point(x,y), cv::Point(std::round(x + fxy.x), std::round(y + fxy.y)), color);
        }
    }
}
