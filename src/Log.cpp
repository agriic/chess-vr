//
//  rtmp_relay
//

#include <chrono>
#include <iostream>
#include <string>
#include "Log.hpp"

using namespace std::chrono;

namespace aic
{
#ifndef DEBUG
    LogLevel Log::threshold = INFO;
#else
    LogLevel Log::threshold = ALL;
#endif

std::mutex coutMx;

Log::Log(LogLevel aLevel)
{
    l.level = aLevel;

    if (aLevel > threshold)
        return;

    l.s.reserve(100);

    {
        auto n = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(n);
        tm *time = localtime(&t);
        char buffer[32];
        strftime(buffer, sizeof(buffer), "%H:%M:%S.", time);

        l.s.append(buffer);

        auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(n);
        auto fraction = n - seconds;
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

        sprintf(buffer, "%03lld | ", milliseconds.count());

        l.s.append(buffer);
    }
}

void Log::flush()
{
    if (!l.s.empty()) {
        std::lock_guard<std::mutex> lock(coutMx);
        std::cout << l.s << "\n";
    }
}
} // namespace aic
