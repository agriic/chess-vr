#include <iostream>
#include <string>
#include "VideoInput.hpp"
#include "Log.hpp"
#include "App.hpp"

using namespace aic;

int main(int argc, const char* argv[])
{
    Log::threshold = ALL;

    std::string path = "0"; // WebCam

    if (argc > 1) {
        path = argv[1];
    }
    
    App app(path);
    app.run();
}
