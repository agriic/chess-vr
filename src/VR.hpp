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

private:
    void run();
    void processFrame(CapturedFrame& frame);

    bool processLines(std::vector<cv::Vec2f>& lines, cv::Mat& frame);
private:
    App& app;

    std::atomic_bool stoped;
    std::thread th;

    ConcurrentQueue<CapturedFrame> frames;
};
}
