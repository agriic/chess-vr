//
//  rtmp_relay
//

#pragma once

#include <string>
#include <thread>
#include <atomic>

namespace aic
{
enum LogLevel
{
    OFF,
    ERR,
    WARN,
    INFO,
    DBG,
    ALL,
    MAX
};

class Log
{
public:

    struct Entry
    {
        std::string s;
        LogLevel level;
    };

    static LogLevel threshold;

    Log(LogLevel aLevel);

    Log(const Log &other)
    {
        l = other.l;
    }

    Log(Log &&other)
    {
        l = std::move(other.l);
    }

    Log &operator=(const Log &other)
    {
        flush();
        l = other.l;

        return *this;
    }

    Log &operator=(Log &&other)
    {
        flush();
        l = std::move(other.l);

        return *this;
    }

    ~Log()
    {
        flush();
    }

    template <typename T>
    Log &operator<<(T val)
    {
        if (l.level <= threshold)
        {
            l.s.append(std::to_string(val));
        }

        return *this;
    }

    Log &operator<<(const std::string &val)
    {
        if (l.level <= threshold)
        {
            l.s.append(val);
        }

        return *this;
    }

    Log &operator<<(const char *val)
    {
        if (l.level <= threshold)
        {
            l.s.append(val);
        }

        return *this;
    }

    Log &operator<<(char *val)
    {
        if (l.level <= threshold)
        {
            l.s.append(val);
        }

        return *this;
    }

private:
    void flush();

    Entry l;
};
} // namespace aic
