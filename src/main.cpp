#include <iostream>

#include "VideoInput.hpp"
#include "Log.hpp"

using namespace aic;

int main()
{
    Log::threshold = ALL;

    VideoInput video("/Users/adumins/projects/personal/chess-vr/samples/chess.avi");
//    VideoInput video("/Users/adumins/projects/personal/chess-vr/samples/chess_img.png");
//    VideoInput video(0);

    CapturedFrame f;
    cv::Mat toShow;

    while (true)
    {
        if (video.getFrame(f))
        {
            cv::imshow("Logs", f.frame);
        }
//
//        if (config.gui) {
//            FrameToShow poppedValue;
//            while (imagesToShow.tryPop(poppedValue)) {
//                cv::imshow(poppedValue.window, poppedValue.frame);
//            }
//
//
//        }

        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    cv::destroyAllWindows();
}
