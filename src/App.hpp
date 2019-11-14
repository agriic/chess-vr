#pragma once

#include <thread>
#include <functional>

#include "VideoInput.hpp"
#include "VR.hpp"
#include "ConcurrentQueue.hpp"

namespace aic
{
class App
{
public:
    App(std::string url);
    App(int camera);
    void run();

    void show(std::string window, cv::Mat frame);

private:
    struct FrameToShow
    {
        std::string window;
        cv::Mat frame;
    };

    VideoInput video;
    VR vr;

    ConcurrentQueue<FrameToShow> imagesToShow;
};
} // namespace aic
