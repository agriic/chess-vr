#include "App.hpp"
#include "Utils.hpp"

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

    cv::namedWindow("GR");
    cv::namedWindow("WC");
    cv::namedWindow("Logs");
    cv::namedWindow("board");
    
    
    
    
    
    
//    cv::namedWindow("P1");
//    cv::moveWindow("P1", 0,500);
//    cv::namedWindow("P2");
//    cv::moveWindow("P2", 640,500);
//    cv::namedWindow("P3");
//    cv::moveWindow("P3", 1160,500);
    
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
                cv::moveWindow("Logs", 0,0);
                cv::moveWindow("board", 640,0);
                cv::moveWindow("GR", 0, 500);
                cv::moveWindow("WC", 640,500);
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
