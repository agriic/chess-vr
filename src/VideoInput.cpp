
#include "VideoInput.hpp"
#include "Log.hpp"

aic::VideoInput::VideoInput(const std::string &videoUrl)
    : videoUrl(videoUrl), stopped(false)
{
    th = std::thread(&VideoInput::run, this);
}

aic::VideoInput::VideoInput(int _captureCamera)
    : useCaptureCamera(true), captureCamera(_captureCamera)
{
    stopped = false;
    th = std::thread(&VideoInput::run, this);
}

aic::VideoInput::~VideoInput()
{
    stopped = true;
    th.join();
}

bool aic::VideoInput::getFrame(aic::CapturedFrame &frame)
{
    return frames.waitAndPopWithTimeout(frame, 100);
}

void aic::VideoInput::run()
{
    VideoType videoType = VideoType::NONE;

    while (!stopped)
    {
        cv::VideoCapture videoCapture;
        if (useCaptureCamera) {
            /*
             *  Set fixed camera capture settings, will need more exp on
             *  actual device's video backend (Raspberry).
             */
            videoCapture.set(CV_CAP_PROP_FPS, 10);
            videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
            videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
            videoCapture.set(CV_CAP_PROP_EXPOSURE, -4); // 80ms exposure time
            videoCapture.set(CV_CAP_PROP_AUTO_EXPOSURE, 0);
            videoCapture.set(cv::CAP_PROP_AUTO_WB, 0);
            videoCapture.set(CV_CAP_PROP_AUTOFOCUS, 0);
            videoCapture.set(CV_CAP_PROP_ISO_SPEED, 100);
            videoCapture.open(captureCamera);
            videoType = VideoType::CAM;
        } else {
            videoCapture.open(videoUrl);
            if (endsWith(videoUrl, "mp4") || endsWith(videoUrl, "avi"))
                videoType = VideoType::VOD;
            else if(endsWith(videoUrl, "jpg") || endsWith(videoUrl, "png"))
                videoType = VideoType::IMAGE;
            else
                videoType = VideoType::STREAM;
        }

        if (!videoCapture.isOpened())
        {
            Log(ERR) << "Video open fails: " << videoUrl;
            std::this_thread::sleep_for(TSecs(7));
            continue;
        }

        if (videoType == VideoType::CAM)
            Log(INFO) << "Video camera opened: " << captureCamera;
        else if(videoType != VideoType::IMAGE)
            Log(INFO) << "Video opened: " << videoUrl;

        int fps = 10;
        if (videoType == VideoType::STREAM
            || videoType == VideoType::VOD
            || videoType == VideoType::CAM)
        {
            fps = (int)videoCapture.get(cv::CAP_PROP_FPS);
            Log(INFO) << "FPS: " << fps;
        }

        auto streamStart = std::chrono::high_resolution_clock::now();

        while (!stopped)
        {
            cv::Mat cframe;
            if (!videoCapture.read(cframe))
            {
                if (videoType == VideoType::STREAM || videoType == VideoType::VOD || videoType == VideoType::CAM)
                    Log(ERR) << "Video: read failed";

                if (++fails > 5)
                    break;
                continue;
            }

            if (videoType == VideoType::VOD || videoType == VideoType::IMAGE)
            {
                auto diff = std::chrono::milliseconds(int(((double)frameNumber / fps) * 1000))
                    - (std::chrono::high_resolution_clock::now() - streamStart);
                if (diff > std::chrono::milliseconds(0))
                {
                    std::this_thread::sleep_for(diff);
                }
            }

            fails = 0;
            frameNumber++;

            CapturedFrame captured(frameNumber, cframe);

            if (!frames.push(captured, MAX_QUEUED_FRAMES))
            {
                // process all frames for vods always
                if (videoType == VideoType::VOD)
                {
                    while (!stopped)
                    {
                        if (frames.push(captured, MAX_QUEUED_FRAMES))
                            break;
                        else
                            std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    }
                }
                else
                {
                    Log(ERR) << "Video queue size limit reached";
                }
            }
        }

        videoCapture.release();

        if (videoType == VideoType::VOD || videoType == VideoType::IMAGE)
            break;
    }
}
