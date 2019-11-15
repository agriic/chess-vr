#include "App.hpp"
#include "Utils.hpp"

namespace aic
{

App::App(const std::string& path) : video(path), vr(*this)
{

}

void App::run()
{
    CapturedFrame f;
    cv::Mat toShow;

    while (true) {
        if (video.getFrame(f)) {
            vr.pushFrame(f);

            // TODO: show something resized - small
            if (f.frameNumber % 2 == 1) {
//                cv::imshow("Logs", f.frame);
            }
        }

        if (true) {
            FrameToShow poppedValue;
            while (imagesToShow.tryPop(poppedValue)) {
                cv::imshow(poppedValue.window, poppedValue.frame);
            }

            if (cv::waitKey(1) == 27) {
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
