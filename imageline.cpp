#include "imageline.h"

ImageLine::ImageLine()
{
    _isHorizontal = false;
    cv::Point *begin = nullptr;
    cv::Point *end = nullptr;
    _points = std::make_pair(begin, end);
}

ImageLine::ImageLine(cv::Point *lineBegin, cv::Point *lineEnd)
{
    _points = std::make_pair(lineBegin, lineEnd);
}

ImageLine::~ImageLine()
{
    clearBeginPoint();
    clearEndPoint();
}

void ImageLine::clearBeginPoint()
{
    if (_points.first != nullptr) {
        delete _points.first;
        _points.first = nullptr;
    }
}

void ImageLine::clearEndPoint()
{
    if (_points.second != nullptr) {
        delete _points.second;
        _points.second = nullptr;
    }
}

void ImageLine::setLineBegin(cv::Point *lineBegin)
{
    _points.first = lineBegin;
}

cv::Point &ImageLine::getLineBegin()
{
    return *_points.first;
}

void ImageLine::setLineEnd(cv::Point *lineEnd)
{
    _points.second = lineEnd;
    if (isCompleted()) {
        _isHorizontal = (std::abs(_points.second->x - _points.first->x) >
                         std::abs(_points.second->y - _points.first->y)) ? true : false;
    }
}

cv::Point &ImageLine::getLineEnd()
{
    return *_points.second;
}

bool ImageLine::isCompleted()
{
    return (_points.first != nullptr && _points.second != nullptr);
}

void ImageLine::draw(cv::Mat &image)
{
    if (isCompleted()) {
        cv::line(image, *_points.first, *_points.second, CV_RGB(0, 255, 0));
    }
}

bool ImageLine::isHorizontal()
{
    return _isHorizontal;
}

