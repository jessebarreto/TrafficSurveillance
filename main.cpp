/******************************************************************************************
 * Universidade de Brasília – UnB
 * Instituto de Ciências Exatas – IE
 * Departamento de Ciência da Computação – CIC
 * Visao Computacional – Professor Fabio Vidal
 *
 * Projeto 3 – Objetivo
 * Identificar padroes de trafico de veiculos dado a imagem de uma camera de transito.
 *
 *  * 1 - Reconhecer veículos
 *
 *  * 2 - Contar os veiculos visualizados
 *
 *  * 3 - Predizer a trajetoria dos veiculos identificados
 *
 * Nome: Jessé Barreto de Barros
 * Matrícula: 17/0067033
 ******************************************************************************************
*/

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "trafficsurveillancecommon.h"

#include "motionsegmentationdetectorhs.h"
#include "bgmogdetector.h"
#include "bgmog2detector.h"
#include "backgroundsubtractor.h"

#include "kalmanfiltertracker.h"
#include "simpletracker.h"
#include "camshifttracker.h"

// BGS Library Implementations
#include "package_bgs/MultiLayer/CMultiLayerBGS.h"
#include "package_bgs/MultiLayer/MultiLayer.h"
#include "package_bgs/PixelBasedAdaptiveSegmenter.h"
#include "package_tracking/BlobTracking.h"
#include "package_analysis/VehicleCouting.h"

// Video Configuration
#define USE_VIDEO 1

#define DEFAULT_VIDEO_NUMBER 1
#define DEFAULT_DATASET_NUMBER 1
#define LOOP_VIDEO false
#define VIDEO_SPEED 300 // ms
#define VIDEO_PROC_TIMER true

/////////////////////////////////////////////////
// Application Configuration

/*!
* List of Possible Detectors
* 0    - Motion Segmentation using Horn-Schunck Optical Flow Algorithm
* 1    - BGS Mean of Gaussians v1
* 2    - BGS Mean of Gaussians v2 (It works better than v1)
* 3    - BGS Multilayer algorithm.
* 4    - BGS Pixel Based Adaptative Segmenter algorithm.
*/
#define DETECTOR_USED 3
#define TRACKER_USED 1


/*!
 * Size of the structuring element when using morphological filter. (Pixels)
 */
#define MORPH_SIZE 3

/*!
 * List of Possible Trackers
 * 0    - Weighted Moving Average Filter
 * 1    - Kalman Filter
 * 2    - CamShift Filter
 */

// Cars Dimension Limits
#define CAR_MIN_SIZE_P100 1
#define CAR_MAX_SIZE_P100 50

#define CAR_UNSEEN_LIMIT 5

////////////////////////////////////////////////
int main(int argc, char **argv)
{
    cv::VideoCapture video; // Capture Object
    cv::Mat frame; // Current Captured Frame
    std::vector<Car *> cars; // List of Cars
    ImageLine mainLine; // Line used to count cars
    VirtualDetection *detector; // The car detector method
    VirtualTrack *tracker; // The car tracker method

    // Open Video or Files
    int ret = USE_VIDEO ? openVideoSource(video, argc, argv, DEFAULT_VIDEO_NUMBER) :
                          openDatasetSource(video, DEFAULT_DATASET_NUMBER);
    if (ret) {
        return ret;
    }

    int carMaxSize = CAR_MIN_SIZE_P100;
    int carMinSize = CAR_MAX_SIZE_P100;
    int morphSize = MORPH_SIZE;

    // Setup
    runSetup(video, frame, mainLine, &morphSize, &carMinSize, &carMaxSize, 25);

    // Get Car MinMax Size, Morph Size as %
    carMaxSize = std::max(frame.rows, frame.cols) * CAR_MAX_SIZE_P100 / 100;
    carMinSize = std::max(frame.rows, frame.cols) * CAR_MIN_SIZE_P100 / 100;
    morphSize = std::max(frame.rows, frame.cols) * MORPH_SIZE / 100;

    switch (DETECTOR_USED) {
    case 4:
        detector = new BackgroundSubtractor(new bgslibrary::algorithms::PixelBasedAdaptiveSegmenter());
        break;
    case 3:
        detector = new BackgroundSubtractor(new MultiLayer);
        break;
    case 2:
        detector = new BGMOG2Detector(5, 5.0, true, 0.01, 250, morphSize);
        break;
    case 1:
        detector = new BGMOGDetector(0.05, 200, morphSize);
        break;
    case 0:
    default:
        detector = new MotionSegmentationDetectorHS(1.0, morphSize);
        break;
    }

    switch (TRACKER_USED) {
    case 2:
        tracker = new CamShiftTracker(64, 180, 64, cv::Size(carMinSize, carMinSize), cv::Size(carMaxSize, carMaxSize), CAR_UNSEEN_LIMIT);
//        break;
    case 1:
        tracker = new KalmanFilterTracker(cv::Size(carMinSize, carMinSize), cv::Size(carMaxSize, carMaxSize), CAR_UNSEEN_LIMIT);
        break;
    case 0:
    default:
        tracker = new SimpleTracker(cv::Size(carMinSize, carMinSize), cv::Size(carMaxSize, carMaxSize), CAR_UNSEEN_LIMIT, 10);
        break;
    }

    // Run Video
    runVideo(video, frame, mainLine, cars, &morphSize, &carMinSize, &carMaxSize, LOOP_VIDEO, VIDEO_SPEED, VIDEO_PROC_TIMER, detector, tracker);

    return 0;
}
