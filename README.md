# Traffic Surveillance System

This repository stores some of the source codes developed in the computer vision course (Visao Computacional @ UnB) during the 1/2017 semester.

This project has as goal a software system to tracking cars on a road obtaining information about their position and the number of cars on a road.

## Bibliography

Antoni B. Chan and Nuno Vasconcelos,
In: IEEE Conference on Computer Vision and Pattern Recognition (CVPR), San Diego, Jun 2005.

## Datasets

http://visal.cs.cityu.edu.hk/downloads/
http://www.ipf.kit.edu/downloads_data_set_AIS_vehicle_tracking.php
http://www.tft.lth.se/en/research/video-analysis/co-operation/data-exchange/

## Compiling and Running

This project was developed and tested in a GNU-Linux computer.
To compile it's necessary to have OpenCV 2.4 installed and 
CMake >= 2.8.

Open terminal:
To compile:
	$ cmake .
	$ make 

To run:
	$ ./TrafficSurveillance