#ifndef TRAFFICSURVEILLANCECOMMON_H
#define TRAFFICSURVEILLANCECOMMON_H

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

// STL
#include <vector>
#include <iostream>
#include <chrono>

// User Libraries
#include "imageline.h"
#include "car.h"
#include "trafficdebug.h"
#include "trafficvideo.h"
#include "trafficimagedataset.h"
#include "virtualdetection.h"
#include "virtualtrack.h"

/*!
 * \brief MAIN WINDOW
 */
static const std::string mainWindow("Video Window");

/*!
 * \brief Método do Projeto 7 Requisito 1
 * Use Optical Flow HS together with Morphologic Transform (Opening) to Identify Cars
 * Parameters - (HS - alpha, Morph - size)
 */
void methodProject7Req1(cv::VideoCapture &video, const cv::Mat &curFrame, double alpha, int morphSize);

/*!
 * \brief Método do Projeto 7 Requisito 2
 * Use Background Subtraction to Identify Cars
 * Parameters - (int algorithm type - 0(default) - MOG2, 1 - MOG, 2 - )
 */
void methodProject7Req2(cv::VideoCapture &video, const cv::Mat &curFrame, int type);

/*!
 * \brief Método Proposto
 */
void mainMethod(cv::VideoCapture &video, const cv::Mat &curFrame);

/*!
 * \brief Run application setup
 */
int runSetup(cv::VideoCapture &video, ImageLine &line, int captureAreaSize);

/*!
 * \brief Run Video in a Loop
 */
void runVideo(cv::VideoCapture &video, cv::Mat &frame, ImageLine &imageLine, std::vector<Car> &cars, bool loop,
              int videoSpeed, bool hasTimer, VirtualDetection *indetificator, VirtualTrack *tracker);

/*!
 * \brief Set a window
 */
void setWindow(const std::string &winName);

/*!
 * \brief drawLineHandler callback function
 */
void drawLineHandler(int event, int x, int y, int flags, void* ptrLine);

#endif // TRAFFICSURVEILLANCECOMMON_H
