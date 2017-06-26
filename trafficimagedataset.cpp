#include "trafficimagedataset.h"

int openDatasetSource(cv::VideoCapture &video, int dataseNumber)
{
    // Open Folder File
    std::string fileNames(__trafficImageDataset__[dataseNumber]);
    printDebug(std::string("Opening default dataset files at ") + fileNames, DEBUGINFO);

    video.open(fileNames);
    if (!video.isOpened()) {
        printDebug(std::string("Could not open dataset files at ") + fileNames, DEBUGERROR);
        return 1;
    }
    return 0;
}
