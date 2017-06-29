#include "trafficsurveillancecommon.h"

#include <tuple>


void setWindow(const std::string &winName)
{
    cv::namedWindow(winName, CV_WINDOW_NORMAL);
    cv::resizeWindow(winName, 640, 480);
}

///////////////////////////////////
static void setupMouseEvent(int event, int x, int y, int flags __attribute__ ((unused)), void *setupData)
{

    // Cast
    std::tuple<ImageLine *, cv::Mat *, cv::Mat *, std::string *> *setupPtr =
            (std::tuple<ImageLine *, cv::Mat *, cv::Mat *, std::string *> *)(setupData);
    ImageLine *line = std::get<0>(*setupPtr);
    cv::Mat *frame = std::get<1>(*setupPtr);
    cv::Mat *showFrame = std::get<2>(*setupPtr);
    std::string *windowName = std::get<3>(*setupPtr);

    if  (event == cv::EVENT_LBUTTONDOWN)
    {
        frame->copyTo(*showFrame); // Clear show image
        cv::imshow(*windowName, *showFrame);
        line->clearBeginPoint();
        line->setLineBegin(new cv::Point(x, y));
    } else if (event == cv::EVENT_LBUTTONUP) {
        line->clearEndPoint();
        line->setLineEnd(new cv::Point(x, y));
        line->draw(*showFrame);
        cv::imshow(*windowName, *showFrame);
    }
}

int runSetup(cv::VideoCapture &video, ImageLine &imageLine, int captureAreaSize __attribute__ ((unused)) = 30)
{
    // Capture 1st frame
    cv::Mat firstFrame;
    video >> firstFrame;
    if (firstFrame.empty()) {
        return 1;
    }

    // Setup Window
    std::string setupWindowName("Setup Window");
    setWindow(setupWindowName);

    // Set Counter Line
    cv::Mat showFrame = firstFrame.clone();
    auto setupPtr = std::make_tuple(&imageLine, &firstFrame, &showFrame, &setupWindowName);
    cv::setMouseCallback(setupWindowName, setupMouseEvent, (void *)(&setupPtr));
    cv::imshow(setupWindowName, showFrame);
    for (;!imageLine.isCompleted();) {
        cv::waitKey(0);
    }

    cv::destroyWindow(setupWindowName);
    setWindow(mainWindow);
    return 0;
}

void runVideo(cv::VideoCapture &video, cv::Mat &frame, ImageLine &imageLine, std::vector<Car> &cars,
              bool loop, int videoSpeed, bool hasTimer, VirtualDetection *indetificator, VirtualTrack *tracker)
{
    cv::Mat identRes;
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

        // Identify Cars in the Scene
        indetificator->process(frame, identRes);

        // Track Cars in the Scene
//        tracker->process();

        // Ends Timer
        auto end = std::chrono::high_resolution_clock::now();
        auto dt = 1.e-6 * std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        // Draw Cars
//        for (Car &car : cars) {

//        }

        if (hasTimer) {
            printDebug(std::string("Processing took ") + std::to_string(dt) + std::string(" ms"), DEBUGINFO);
        }

        imageLine.draw(frame);
        cv::imshow(mainWindow, frame);
    }
}
