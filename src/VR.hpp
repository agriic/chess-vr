#pragma once

#include "ConcurrentQueue.hpp"
#include "Utils.hpp"

namespace aic
{
class App;

class VR
{
    struct Line {
        cv::Vec2f detected;
        float angle;
        cv::Point start;
    };

public:
    VR(App& app);
    ~VR();

    void pushFrame(CapturedFrame& frame);

public:

    const int SQS = 64; // square size
    const int BS = SQS * 8; // board size

private:
    void run();
    void processFrame(CapturedFrame& frame);

    std::vector<cv::Point2f> findCorners(cv::Mat& frame);

    bool findOrthogonals(std::vector<cv::Vec2f>& lines);

    void warpBoard(std::vector<cv::Point2f>& croners, cv::Mat& source, cv::Mat& out, bool rotationSearch = false);

    void findRotation(cv::Mat& warpedImage);

private:
    App& app;

    std::atomic_bool stoped;
    std::thread th;

    ConcurrentQueue<CapturedFrame> frames;

    int rotation = -1;
};
}
