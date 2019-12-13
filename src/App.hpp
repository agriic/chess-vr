#pragma once

#include <thread>
#include <functional>

#include "VideoInput.hpp"
#include "VR.hpp"
#include "ConcurrentQueue.hpp"
#include "Game.hpp"

namespace aic
{
class App
{
public:
    App(const std::string& path);
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
//    Game game;

    ConcurrentQueue<FrameToShow> imagesToShow;
};
} // namespace aic
