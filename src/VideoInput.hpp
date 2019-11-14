#pragma once

#include "ConcurrentQueue.hpp"
#include "Utils.hpp"

namespace aic
{
class VideoInput
{
public:
    explicit VideoInput(const std::string &videoUrl);
    explicit VideoInput(int captureCamera);
    ~VideoInput();

    VideoInput(const VideoInput &) = delete;
    VideoInput &operator=(VideoInput const &) = delete;

    void run();

    bool getFrame(CapturedFrame &frame);

private:

    enum class VideoType {
        NONE,
        CAM,
        STREAM,
        VOD,
        IMAGE
    };

    std::string videoUrl;
    bool useCaptureCamera = false;
    int captureCamera = 0;

    std::atomic<bool> stopped{};
    std::thread th;
    ConcurrentQueue<CapturedFrame> frames;

    uint64_t fails = 0;
    uint64_t frameNumber = 0;
};
} // namespace aic
