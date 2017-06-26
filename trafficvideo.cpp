#include "trafficvideo.h"

#include "trafficdebug.h"

int openVideoSource(cv::VideoCapture &video, int argc, char **argv, int defaultVideoNumber)
{
    // Open Video File
    std::string fileName;
    if (argc < 2) {
        fileName = __trafficVideoFiles__[defaultVideoNumber];
        printDebug(std::string("Parameter not found - Opening default video file at ") + fileName, DEBUGWARNING);
    } else {
        fileName = argv[1];
    }
    video.open(fileName);
    if (!video.isOpened()) {
        printDebug(std::string("Could not open video file at ") + fileName, DEBUGERROR);
        return 1;
    }
    return 0;
}

