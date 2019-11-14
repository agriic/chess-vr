#include <iostream>
#include <string>
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

    std::string path("/Users/danpetrov/GIT/chess-vr/samples/chess.avi");
    if (fileExists(path)) {
        App app(path);
        app.run();
    }
//    App app("/Users/adumins/projects/personal/chess-vr/samples/chess_img.png");

}
