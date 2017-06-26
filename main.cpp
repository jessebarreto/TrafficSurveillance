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

#include "trafficsurveillancecommon.h"

// Video Configuration
#define USE_VIDEO 0
#define DEFAULT_VIDEO_NUMBER 0
#define DEFAULT_DATASET_NUMBER 0
#define LOOP_VIDEO false
#define VIDEO_SPEED 100 // ms
#define VIDEO_PROC_TIMER true

// MAIN WINDOW
static const std::string mainWindow("Video Window");

// Run Setup
void runSetup();
void runSetup()
{
    setWindow(mainWindow);

    // SETUP PROCESS HERE

}

// Run Video Function
void runVideo(cv::VideoCapture &video, cv::Mat &frame, std::vector<Car> &cars, bool loop, int videoSpeed, bool hasTimer);
void runVideo(cv::VideoCapture &video, cv::Mat &frame, std::vector<Car> &cars, bool loop, int videoSpeed, bool hasTimer)
{
    for (;;) {
        video >> frame;

        // Loop the video
        if (frame.empty()) {
            if (loop) {
                video.set(CV_CAP_PROP_POS_FRAMES, 0);
                continue;
            } else {
                break;
            }
        }

        // Exit if user press space
        if (cv::waitKey(videoSpeed) >= 0) {
            break;
        }

        // Start Timer
        auto start = std::chrono::high_resolution_clock::now();

        // PROCESS HERE


        // Ends Timer
        auto end = std::chrono::high_resolution_clock::now();
        auto dt = 1.e-6 * std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        // Draw Cars
//        for (Car &car : cars) {

//        }

        if (hasTimer) {
            printDebug(std::string("Processing took ") + std::to_string(dt) + std::string(" ms"), DEBUGINFO);
        }

        cv::imshow(mainWindow, frame);
    }
}

int main(int argc, char **argv)
{
    static cv::VideoCapture video; // Capture Object
    static cv::Mat frame; // Current Captured Frame
    static std::vector<Car> cars; // List of Cars

    // Open Video or Files
    int ret = USE_VIDEO ? openVideoSource(video, argc, argv, DEFAULT_VIDEO_NUMBER) :
                          openDatasetSource(video, DEFAULT_DATASET_NUMBER);
    if (ret) {
        return ret;
    }

    // Setup
    runSetup();

    // Run Video
    runVideo(video, frame, cars, LOOP_VIDEO, VIDEO_SPEED, VIDEO_PROC_TIMER);


    return 0;
}
