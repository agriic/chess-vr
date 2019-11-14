#pragma once

#define LOCK(X) std::lock_guard<std::mutex> X ## _lock(X)

namespace aic
{


    typedef std::chrono::steady_clock Timer;
    typedef std::chrono::system_clock Date;
    inline auto TimerMilliseconds() { return std::chrono::time_point_cast<std::chrono::milliseconds>(Timer::now()).time_since_epoch().count(); }

    typedef decltype(std::chrono::seconds(0)) TSecs;
    typedef decltype(std::chrono::minutes(0)) TMins;

    struct CapturedFrame
    {
        CapturedFrame()
        { }

        CapturedFrame(uint64_t _frameNumber, cv::Mat& _frame)
                : frameNumber(_frameNumber)
                , timer(Timer::now())
                , date(Date::now())
                , frame(_frame)
        { } 

        uint64_t frameNumber = 0;
        Timer::time_point timer;
        Date::time_point date;
        cv::Mat frame;
    };

    std::string getAlphaNumeric(std::string input);
    bool endsWith(const std::string& str, const std::string& needle);
    bool fileExists(const std::string& name);
    float euclideanDist(cv::Point2f& p, cv::Point2f& q);
}
