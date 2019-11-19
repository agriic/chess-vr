#include "App.hpp"
#include "Utils.hpp"

#include "Log.hpp"

namespace aic
{

App::App(const std::string& path) : video(path), vr(*this), game(*this)
{

}

void App::run()
{
    CapturedFrame f;
    cv::Mat toShow;

    VRRequest action = VRRequest::NONE;
    bool stop = false;
    while (!stop) {
        if (video.getFrame(f)) {
            vr.pushFrame(f);

            // TODO: show something resized - small
            if (f.frameNumber % 2 == 1) {
                cv::imshow("Logs", f.frame);
            }
        }

        {
            FrameToShow poppedValue;
            while (imagesToShow.tryPop(poppedValue)) {
                cv::imshow(poppedValue.window, poppedValue.frame);
            }

            auto key = cv::waitKey(1);
            switch (key) {
                case ' ':
                    action = VRRequest::MOVE;
                    Log(DBG) << "Move";
                    break;
                case 's':
                    action = VRRequest::GAME_START;
                    Log(DBG) << "Game Start";
                case 27: // ESC
                case 'q':
                    stop = true;
                    break;
                default:
                    break;
            }
        }
    }

    cv::destroyAllWindows();
}

void App::show(std::string window, cv::Mat frame)
{
    imagesToShow.push(FrameToShow{window, frame.clone()}, 50);
}

}
