#include <iostream>
#include <string>
#include "VideoInput.hpp"
#include "Log.hpp"
#include "App.hpp"

using namespace aic;

int main(int argc, const char* argv[])
{
    Log::threshold = ALL;

    std::string path = "/Users/danpetrov/GIT/chess-vr/samples/chess.avi"; // WebCam

    if (argc > 1) {
        path = argv[1];
    }
    
    App app(path);
    app.run();
}
