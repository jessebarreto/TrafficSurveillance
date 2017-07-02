#######################################################################
#
# THIS FILE MAKES THE COMPILATION OF CODE DEVELOPED IN THIS SEMESTER AUTOMATIC.
# BY jesseh.barreto@gmail.com
#
#
# TO USE JUST ADD A NEW SUB_DIRECTORY WITH A CMAKELIST.TXT INSIDE.
# WITH:
#       add_executable(bin_name source_code)
#       target_link_libraries(bin_name libraries)
#
#######################################################################

cmake_minimum_required(VERSION 2.8)
project(TrafficSurveillanceSystem)


# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

## Find OpenCV
find_package(OpenCV REQUIRED)
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
include_directories(SYSTEM "${OpenCV_INCLUDE_DIRS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -std=c++0x")
set(CMAKE_CXX_STANDARD 11)

## Copy data files from source folder to binary folder
file(COPY data DESTINATION .)

## Compile Executable
add_executable(	TrafficSurveillance #Executable Binary
                main.cpp #Source List
                imageline.cpp
                car.cpp
                carkf.cpp
                trafficsurveillancecommon.cpp
                trafficdebug.cpp
                trafficvideo.cpp
                trafficimagedataset.cpp
                flowdrawer.cpp
                motionsegmentationdetectorhs.cpp
                bgmog2detector.cpp
                bgmogdetector.cpp
                kalmanfiltertracker.cpp
                simpletracker.cpp
                virtualdetection.h #Includes
                virtualtrack.h
                )
target_link_libraries(TrafficSurveillance ${OpenCV_LIBS})
