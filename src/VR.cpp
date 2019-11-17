#include "VR.hpp"

#include "Utils.hpp"
#include "App.hpp"
#include "Log.hpp"

#include <algorithm>
#include <numeric>
#include <functional>

#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

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

        cv::Point pt1, pt2; // start and end points of drawn line segment
        
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        // calculate start and end points of line segment in Cartesian system
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        cv::line( frame, pt1, pt2, col, thicknes, cv::LINE_AA);
    }
}


//Find point (x,y) where two parameterized lines intersect :p Returns 0 if lines are parallel
static cv::Point parametricIntersect(cv::Vec2f line1, cv::Vec2f line2)
{
    float r1 = line1[0], t1 = line1[1];
    float r2 = line2[0], t2 = line2[1];

    float ct1=cosf(t1);     //matrix element a
    float st1=sinf(t1);     //b
    float ct2=cosf(t2);     //c
    float st2=sinf(t2);     //d
    float d=ct1*st2-st1*ct2;        //determinative (rearranged matrix for inverse)

    if(d!=0.0f) {
        return cv::Point((int)((st2*r1-st1*r2)/d), (int)((-ct2*r1+ct1*r2)/d));
    } else { //lines are parallel and will NEVER intersect!
        return cv::Point(-1,-1);
    }
}



std::vector<cv::Point2f> VR::findCorners(cv::Mat &frame)
{
    std::vector<cv::Point2f> corners;

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
        return corners;
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

    if (lines.size() != 18) return corners;

    // Find corners
    auto p1 = parametricIntersect(lines[0], lines[17]);
    auto p2 = parametricIntersect(lines[0], lines[9]);
    auto p3 = parametricIntersect(lines[8], lines[9]);
    auto p4 = parametricIntersect(lines[8], lines[17]);

    corners.push_back(p1);
    corners.push_back(p2);
    corners.push_back(p3);
    corners.push_back(p4);

//    cv::circle(frame, p1, 3, cv::Scalar(0, 255, 0), -1);
//    cv::circle(frame, p2, 3, cv::Scalar(0, 255, 0), -1);
//    cv::circle(frame, p3, 3, cv::Scalar(0, 255, 0), -1);
//    cv::circle(frame, p4, 3, cv::Scalar(0, 255, 0), -1);

//    Log(DBG) << p1 << " " << p2 << " " << p3 << " " << p4;

//    drawLines(lines, frame, cv::Scalar(0,0,255),1);
//    app.show("FR 2", frame);

//    findOrthogonals(lines);

    return corners;
}

bool VR::findOrthogonals(std::vector<cv::Vec2f>& lines)
{
    int found = 0; // accum of found orthogonal pairs
    for (size_t i = 0; i < lines.size(); i++)
    {
        for (size_t j = i; j < lines.size(); j++)
        {
            if (abs(lines[i][1] - lines[j][1]) > 0.78)
            {
                ++found;
            }
        }
    }
    //Log(INFO) << "Found orthogonal line pairs :\n" << found;
    return (found > 0);
}

double calcWhiteness(cv::Mat& gray)
{
    double sum = 0;
    for( int y = 0; y < gray.rows; y++ ) {
        uint8_t* rs = &gray.at<uint8_t>(y,0);
        for( int x = 0; x < gray.cols; x++ ) {
            sum += rs[x];
        }
    }

    sum /= gray.rows * gray.cols;

    return sum;
}

void VR::findRotation(cv::Mat& warpedImage)
{
    cv::Mat gs;
    cv::cvtColor(warpedImage, gs, cv::COLOR_BGR2GRAY);

    cv::Mat corner;
    corner = warpedImage(cv::Rect(0,0,SQS,SQS));
    auto r = 0;
    auto w = calcWhiteness(corner);
    corner = warpedImage(cv::Rect(BS-SQS,0,SQS,SQS));
    auto w2 = calcWhiteness(corner);
    if (w2 > w) {
        r = 1;
        w = w2;
    }
    corner = warpedImage(cv::Rect(BS-SQS,BS-SQS,SQS,SQS));
    auto w3 = calcWhiteness(corner);
    if (w3 > w) {
        r = 2;
        w = w3;
    }
    corner = warpedImage(cv::Rect(0,BS-SQS,SQS,SQS));
    auto w4 = calcWhiteness(corner);
    if (w4 > w) {
        r = 3;
        w = w4;
    }

    rotation = r;
}

void VR::warpBoard(std::vector<cv::Point2f> &corners, cv::Mat& src, cv::Mat& out, bool rotationSearch)
{
    if (rotation < 0 && !rotationSearch) {
        warpBoard(corners, src, out, true);
        findRotation(out);
    }

    std::vector<cv::Point2f> pts_dst;
    pts_dst.resize(4);

    auto r = 0;

    if (rotation > 0) {
        r = rotation;
    }

    pts_dst[(0 + r) % 4] = cv::Point2f(0, 0);
    pts_dst[(1 + r) % 4] = cv::Point2f(0, BS);
    pts_dst[(2 + r) % 4] = cv::Point2f(BS, BS);
    pts_dst[(3 + r) % 4] = cv::Point2f(BS, 0);

       // Calculate Homography
    cv::Mat h = cv::findHomography(corners, pts_dst);

       // Warp source image to destination based on homography
    cv::warpPerspective(src, out, h, cv::Size(BS, BS));

//    Log(DBG) << im_out.rows << " " << im_out.cols;

//    app.show("warped", out);
}

void VR::boardCell(cv::Mat &src, cv::Mat &dst, char column, int lvl)
{
    int x(VR::SQS * ((int)column - 'A'));
    int y(VR::SQS * (abs(lvl - 8)));

    dst = src(cv::Rect(x, y, VR::SQS, VR::SQS));
}

void VR::processFrame(CapturedFrame& cframe)
{
    if (cframe.frame.empty()) return;

    cv::Mat& frame = cframe.frame;

    cv::Mat blurred;
    cv::GaussianBlur(frame, blurred, cv::Size(3,3), 0);

    auto corners = findCorners(blurred);

    if (corners.empty()) return;

    cv::Mat dst;
    warpBoard(corners, frame, dst);

    cv::Mat cannys, cell;
    cv::Canny(dst, cannys, 20, 100);
    boardCell(cannys, cell, 'A', 2); // pawn on a2

    app.show("Cell", cell);
    app.show("WC", cannys);

}

}
