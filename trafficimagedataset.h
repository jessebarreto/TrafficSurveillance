#ifndef TRAFFICIMAGEDATASET_H
#define TRAFFICIMAGEDATASET_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "trafficdebug.h"

const static std::string __trafficImageDataset__[] =  {"data/KIT_AIS_Dataset/MunichAutobahn01/%03d.png",
                                                       "data/KIT_AIS_Dataset/MunichCrossroad01/%03d.png",
                                                       "data/KIT_AIS_Dataset/MunichCrossroad02/%03d.png",
                                                       "data/KIT_AIS_Dataset/MunichStreet01/%03d.png",
                                                       "data/KIT_AIS_Dataset/MunichStreet02/%03d.png",
                                                       "data/KIT_AIS_Dataset/MunichStreet03/%03d.png",
                                                       "data/KIT_AIS_Dataset/MunichStreet04/%03d.png",
                                                       "data/KIT_AIS_Dataset/StuttgartAutobahn01/%03d.png",
                                                       "data/KIT_AIS_Dataset/StuttgartCrossroad01/%03d.png"};

/*!
 * \brief Opens a video from a dataset of images
 */
int openDatasetSource(cv::VideoCapture &video, int dataseNumber);

#endif // TRAFFICIMAGEDATASET_H
