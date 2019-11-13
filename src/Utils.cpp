#include "Utils.hpp"

#include <sys/stat.h>

std::string aic::getAlphaNumeric(std::string input)
{
    for (int i = 0; i < input.size(); i++) {
        auto c = input[i];
        if (!((c >= '0' && c <= '9')
            || (c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z'))) {
            input[i] = '.';
        }
    }

    input.erase(std::remove(input.begin(), input.end(), '.'), input.end());
    return input;
}

bool aic::endsWith(const std::string& str, const std::string& needle)
{
    if (str.length() >= needle.length()) {
        return (0 == str.compare (str.length() - needle.length(), needle.length(), needle));
    } else {
        return false;
    }
}

bool aic::fileExists(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

float aic::euclideanDist(cv::Point2f& p, cv::Point2f& q) {
    cv::Point2f diff = p - q;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

