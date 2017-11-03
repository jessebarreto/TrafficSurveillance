#ifndef TRAFFICVIDEO_H
#define TRAFFICVIDEO_H

#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

const static std::string __trafficVideoFiles__[] =  {   "data/cam1_cars10.mp4",
                                                        "data/cam1_cars10.mp4",
                                                        "data/cam2_cars101.avi",
                                                        "data/cam3_cars50.mp4",
                                                        "data/cam4_cars28.wmv",
                                                        "data/cam5_cars17.MOV",
                                                        "data/cam6_cars29.m4v",
                                                        "data/cam7_cars9.MOV",
                                                        "data/cam8_cars20.MP4",
                                                        "data/cam9_cars12.MP4",
                                                        "data/cam10_cars81.mp4",
                                                        "data/cam11_cars84.mp4",
                                                        "data/cam12_cars125.mov",
                                                        "data/cam13_cars35.avi",
                                                        "data/cam14_cars80.mp4",
                                                    };

//const static int __trafficCarPerVideo__[] = {10, 129, 57};

/*!
 * \brief Opens a video from a source video.
 */
int openVideoSource(cv::VideoCapture &video, int argc, char **argv, int defaultVideoNumber);

#endif // TRAFFICVIDEO_H
