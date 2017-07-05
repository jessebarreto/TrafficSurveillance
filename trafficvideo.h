#ifndef TRAFFICVIDEO_H
#define TRAFFICVIDEO_H

#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

const static std::string __trafficVideoFiles__[] =  {
                                                        "data/viptraffic.mp4",
                                                        "data/Bsb-EPGU.mov",
                                                        "data/highwayI_raw.avi",
                                                        "data/rene_video.mov",
                                                        "data/Camera_1.mp4",
                                                        "data/Camera_2.mp4",
                                                        "data/Camera_3.mp4",
                                                        "data/Camera_4.mp4"
                                                    };

const static int __trafficCarPerVideo__[] = {10, 129, 57};

/*!
 * \brief Opens a video from a source video.
 */
int openVideoSource(cv::VideoCapture &video, int argc, char **argv, int defaultVideoNumber);

#endif // TRAFFICVIDEO_H
