#include <iostream>

#include "VideoInput.hpp"
#include "Log.hpp"
#include "App.hpp"

using namespace aic;

int main()
{
    Log::threshold = ALL;

//    VideoInput video("/Users/adumins/projects/personal/chess-vr/samples/chess.avi");
//    VideoInput video("/Users/adumins/projects/personal/chess-vr/samples/chess_img.png");
//    VideoInput video(0);

    App app("/Users/adumins/projects/personal/chess-vr/samples/chess.avi");
//    App app("/Users/adumins/projects/personal/chess-vr/samples/chess_img.png");
    app.run();

}
