#include "backgroundsubtractor.h"

BackgroundSubtractor::BackgroundSubtractor(IBGS *bgRef) :
    _bgs(bgRef)
{
}

BackgroundSubtractor::~BackgroundSubtractor()
{
    if (_bgs) {
        delete _bgs;
    }
}

void BackgroundSubtractor::process(const cv::Mat &frame, cv::Mat &result)
{
    _bgs->process(frame, result, _imgBgkModel);
}

void BackgroundSubtractor::init()
{
}
