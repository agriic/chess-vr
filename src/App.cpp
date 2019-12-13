#include "App.hpp"
#include "Utils.hpp"
#include "Chess.hpp"

#include "Log.hpp"

namespace aic
{

App::App(const std::string& path) : video(path), vr(*this)
{

}

void App::run()
{
    CapturedFrame f;
    cv::Mat toShow;

    cv::namedWindow("Logs");
    cv::moveWindow("Logs", 0,0);
    cv::namedWindow("GR");
    cv::moveWindow("GR", 640,0);
    cv::namedWindow("WC");
    cv::moveWindow("WC", 1160,0);
    
    VRRequest action = VRRequest::NONE;
    bool stop = false;
    while (!stop) {

        if (video.getFrame(f)) {
            f.request = action;
            vr.pushFrame(f);
            action = VRRequest::NONE;

            // TODO: show something resized - small
            if (f.frameNumber % 2 == 1) {
                cv::Mat resized;
                cv::resize(f.frame, resized, cv::Size(f.frame.cols / 2, f.frame.rows / 2));
                cv::imshow("Logs", resized);
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
                    Log(DBG) << "Move Req";
                    break;
                case 's':
                    action = VRRequest::GAME_START;
                    Log(DBG) << "Game Start Req";
                    break;
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
