/******************************************************************************************
 * Universidade de Brasília – UnB
 * Instituto de Ciências Exatas – IE
 * Departamento de Ciência da Computação – CIC
 * Visao Computacional – Professor Fabio Vidal
 *
 * Projeto 3 – Objetivo
 * Identificar padroes de trafico de veiculos dado a imagem de uma camera de transito.
 *
 *  * 1 - Reconhecer veiculos
 *
 *  * 2 - Contar os veiculos visualizados
 *
 *  * 3 - Predizer a trajetoria dos veiculos identificados
 *
 *  * 4 - Estimar dado a taxa de atualizacao do video a velocidade desses veiculos
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

#include "kalmanfiltertracker.h"
#include "simpletracker.h"

// Video Configuration
#define USE_VIDEO 1
#define DEFAULT_VIDEO_NUMBER 1
#define DEFAULT_DATASET_NUMBER 5
#define LOOP_VIDEO false
#define VIDEO_SPEED 300 // ms
#define VIDEO_PROC_TIMER false

#define DETECTOR_USED 2
#define MORPH_SIZE 4
#define TRACKER_USED 1
#define CAR_MIN_SIZE_PX 2

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

    switch (DETECTOR_USED) {
    case 2:
        detector = new BGMOG2Detector(5, 5.0, true, 0.01, 250, MORPH_SIZE);
        break;
    case 1:
        detector = new BGMOGDetector(0.05, 200, MORPH_SIZE);
        break;
    case 0:
    default:
        detector = new MotionSegmentationDetectorHS(1.0, MORPH_SIZE);
        break;
    }

    switch (TRACKER_USED) {
    case 1:
        tracker = new KalmanFilterTracker(cv::Size(CAR_MIN_SIZE_PX, CAR_MIN_SIZE_PX), cv::Size(40, 40), 5);
        break;
    case 0:
    default:
        tracker = new SimpleTracker(cv::Size(CAR_MIN_SIZE_PX, CAR_MIN_SIZE_PX), cv::Size(40, 40), 5, 10);
        break;
    }

    // Setup
    runSetup(video, mainLine, 25);

    // Run Video
    runVideo(video, frame, mainLine, cars, LOOP_VIDEO, VIDEO_SPEED, VIDEO_PROC_TIMER, detector, tracker);

    return 0;
}
