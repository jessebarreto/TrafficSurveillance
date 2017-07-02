#include "trafficsurveillancecommon.h"

#include <tuple>
#include <math.h>


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

void runVideo(cv::VideoCapture &video, cv::Mat &frame, ImageLine &imageLine, std::vector<Car *> &cars,
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
        int counter = tracker->process(frame, identRes, imageLine, cars);

        // Ends Timer
        auto end = std::chrono::high_resolution_clock::now();
        auto dt = 1.e-6 * std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

        // Draw Cars
        for (Car *car : cars) {
            car->drawCar(frame);
        }

        if (hasTimer) {
            printDebug(std::string("Processing took ") + std::to_string(dt) + std::string(" ms"), DEBUGINFO);
        }

        // Draw number of cars
        cv::putText(frame, std::to_string(counter), cv::Point(0,frame.rows - 20), cv::FONT_HERSHEY_PLAIN, 0.5,
                    cv::Scalar(127, 255, 255));

        imageLine.draw(frame);
        cv::imshow(mainWindow, frame);
    }
}

void getVectorPolarCoord(const cv::Point &src, const cv::Point &dst, double *magnitude, double *angle)
{
    double dx = dst.x - src.x;
    double dy = dst.y - src.y;

    *magnitude = std::sqrt(dx*dx + dy*dy);

    if (dy > 0) {
        *angle = std::atan2(-dx, dy);
    } else if (dy == 0) {
        if (dx < 0) {
            *angle = M_PI_2;
        } else if (dx > 0) {
            *angle = -1.0 * M_PI_2;
        } else {
            *angle = 0.0;
        }
    } else {
        if (dx < 0) {
            *angle = M_PI - std::atan2(dx, dy);
        } else if (dx > 0) {
            *angle = -1.0 * M_PI - std::atan2(dx, dy);
        } else {
            *angle = M_PI;
        }
    }
}

bool carCrossesLine(Car &car, ImageLine &imageLine)
{
    if (car.getPositionsSize() <= 1) {
        return false;
    }

    bool ret;
    float carX0 = car.getFirstPosition().x;
    float carY0 = car.getFirstPosition().y;
    float carX1 = car.getLastPosition().x;
    float carY1 = car.getLastPosition().y;

    float lineX0 = imageLine.getLineBegin().x;
    float lineY0 = imageLine.getLineBegin().y;
    float lineX1 = imageLine.getLineEnd().x;
    float lineY1 = imageLine.getLineEnd().y;

    cv::Point2f intersection;
    bool hasIntersection = getLineIntersection(cv::Point2f(carX0, carY0), cv::Point2f(carX1, carY1), cv::Point2f(lineX0, lineY0),
                                               cv::Point2f(lineX1, lineY1), intersection);
    bool isInsideOfRange = false;
    if (hasIntersection) {
        // check whether the line is vertical or horizontal
        if (imageLine.isHorizontal()) { // Horizontal
            // Car direction
            if ((carY1 - carY0) > 0) { // Down
                ret = (carY1 > intersection.y && carY0 < intersection.y);
            } else { // Up
                ret = (carY0 < intersection.y && carY0 > intersection.y);
            }
            isInsideOfRange = (carX1 <= lineX1 && carX1 >= lineX0);
        } else { // Vertical
            // Car direction
            if ((carX1 - carX0) > 0) { // Right
                ret = (carX1 > intersection.x && carX0 < intersection.x);
            } else { // Left
                ret = (carX1 < intersection.x && carX0 > intersection.x);
            }
            isInsideOfRange = (carY1 <= lineY1 && carY1 >= lineY0);
        }
    }

    return ret & isInsideOfRange;
}

bool getLineIntersection(const cv::Point2f &a1, const cv::Point2f &b1, const cv::Point2f &a2,
                         const cv::Point2f &b2, cv::Point2f &intersection) {
    double A1 = b1.y - a1.y;
    double B1 = a1.x - b1.x;
    double C1 = (a1.x * A1) + (a1.y * B1);

    double A2 = b2.y - a2.y;
    double B2 = a2.x - b2.x;
    double C2 = (a2.x * A2) + (a2.y * B2);

    double det = (A1 * B2) - (A2 * B1);

    if (std::abs(det) > 0.01/*eps*/) {
        intersection.x = static_cast<float>(((C1 * B2) - (C2 * B1)) / (det));
        intersection.y = static_cast<float>(((C2 * A1) - (C1 * A2)) / (det));

        return true;
    }

    return false;
}
