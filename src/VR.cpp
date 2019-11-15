#include "VR.hpp"

#include "Utils.hpp"
#include "App.hpp"
#include "Log.hpp"

#include <algorithm>
#include <numeric>
#include <functional>

namespace aic
{
VR::VR(App& app)
    : app(app)
    , stoped(false)
{
    th = std::thread(&VR::run, this);
}

VR::~VR()
{
    stoped = true;
    th.join();
}

void VR::run()
{
    while (!stoped) {
        CapturedFrame frame;

        if (!frames.waitAndPopWithTimeout(frame, 100)) continue;

        processFrame(frame);
    }
}

void VR::pushFrame(CapturedFrame& frame)
{
    if (!frames.push(frame, MAX_QUEUED_FRAMES)) {
//        LOG->warn("VR queue size limit reached. Frame skipped.");
    }
}

static void drawLines(std::vector<cv::Vec2f>& lines, cv::Mat& frame, cv::Scalar col, int thicknes)
{
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];

        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        cv::line( frame, pt1, pt2, col, thicknes, cv::LINE_AA);
    }
}

bool VR::findCorners(cv::Mat &frame)
{
    cv::Mat cannys;
    cv::Canny(frame, cannys, 100, 300);


    // Standard Hough Line Transform
    std::vector<cv::Vec2f> lines; // will hold the results of the detection
    HoughLines(cannys, lines, 1, CV_PI/180, 140, 0, 0); // runs the actual detection

    std::sort(lines.begin(), lines.end(), [](const cv::Vec2f& a, const cv::Vec2f& b)
    {
        return a[0] < b[0];
    });

//    Log(DBG) << "-----------------------------";
//    for( size_t i = 0; i < lines.size(); i++ ) {
//        Log(DBG) << lines[i][1] << " " << lines[i][0];
//    }

    // smooth out angles
    int start = 0, end = 0;
    for( size_t i = 0; i < lines.size(); i++ )
    {
        bool smallDist = fabs(lines[i][0] - lines[start][0]) <= 12;

        if (smallDist && lines.size() - 1 == i) end = i;

        if (end == i || !smallDist)
        {
            float avgAS = 0, avgDS = 0;
            for (int j = start; j < end + 1; j++) {
                avgAS += lines[j][1];
                avgDS += lines[j][0];
            }

            float avgA = avgAS / (end + 1 - start);
            float avgD = avgDS / (end + 1 - start);

            for (int j = start; j < end + 1; j++) {
                lines[j][1] = avgA;
                lines[j][0] = avgD;
            }

            start = i;
        }

        end = i;
    }

    // delete even lines
    for (auto it = lines.begin(); it != lines.end(); ) {
        auto next = it + 1;
        if (next != lines.end()) {
            if ((*it)[0] == (*next)[0] && (*it)[1] == (*next)[1]) {
                it = lines.erase(it);
                continue;
            }
        }

        it++;
    }

//    Log(DBG) << "-----------------------------";
//    for( size_t i = 0; i < lines.size(); i++ ) {
//        Log(DBG) << lines[i][1] << " " << lines[i][0];
//    }

    std::vector<double> dists;
    for( size_t i = 0; i < lines.size() - 1; i++ ) {
        if (fabs(lines[i][1] - lines[i+1][1]) < 0.2) {
            dists.push_back(fabs(lines[i+1][0] - lines[i][0]));
        }
    }

    if (dists.size() < 16) {
        return false;
    }

    auto toRemove = (dists.size() - 14) / 2;

    std::sort(dists.begin(), dists.end());
    dists.erase(dists.begin(), dists.begin()+toRemove);
    dists.erase(dists.end()-toRemove, dists.end());

    double rootedAvg = std::accumulate(dists.begin(), dists.end(), 0) / dists.size();

    // delete not similar distances
    for (auto it = lines.begin(); it != lines.end(); ) {
        if (it == lines.begin() || fabs((*(it-1))[1] - (*it)[1]) > 0.2) {
            // first line -> check forward
            if (it + 1 == lines.end() || fabs((*(it+1))[1] - (*it)[1]) > 0.2) {
                it = lines.erase(it);
                continue;
            }

            // next line angle is ok

            if (fabs(fabs((*(it+1))[0] - (*it)[0]) - rootedAvg) > rootedAvg * 0.3) {
                it = lines.erase(it);
                continue;
            }
        } else { // check back
            if (fabs(fabs((*(it-1))[0] - (*it)[0]) - rootedAvg) > rootedAvg * 0.3) {
                it = lines.erase(it);
                continue;
            }
        }

        it++;
    }

//    Log(DBG) << "-----------------------------";
//    for( size_t i = 0; i < lines.size(); i++ ) {
//        Log(DBG) << lines[i][1] << " " << lines[i][0];
//    }

    drawLines(lines, frame, cv::Scalar(0,0,255),2);
    app.show("FR 2", frame);

    return true;
}

void VR::processFrame(CapturedFrame& cframe)
{
    if (cframe.frame.empty()) return;

    cv::Mat& frame = cframe.frame;

    cv::Mat blurred;
    cv::GaussianBlur(frame, blurred, cv::Size(3,3), 0);

    if (!findCorners(blurred)) return;
}

}
