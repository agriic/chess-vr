#include "VR.hpp"

#include <algorithm>
#include <numeric>
#include <functional>

#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

#include "Utils.hpp"
#include "App.hpp"
#include "Log.hpp"
#include "Game.hpp"

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
    if (frame.request != VRRequest::NONE) {
        // TODO: save frame
        frames.push(frame);
    } else {
        frames.push(frame, MAX_QUEUED_FRAMES);
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

bool VR::cellHasPiece(cv::Mat &src, int thresh=150)
{
    int margin = 10;
    cv::Mat roi(src(cv::Rect(margin, margin,src.size().width - margin,src.size().height - margin)));

    return cv::countNonZero(roi) > thresh;
}

static std::string p(int pos) {
    std::string rez = "[";
    rez += (char)('A' + (pos % 8));
    rez += std::to_string(8 - (pos / 8));
    rez += "]";
    
    return rez;
}

void VR::processFrame(CapturedFrame& cframe)
{
    if (cframe.frame.empty()) return;

    if (cframe.request != VRRequest::NONE) {
        currentRequest = cframe.request;
    }
    
    if (currentRequest == VRRequest::NONE) return;
    
    cv::Mat& frame = cframe.frame;

    cv::Mat blurred;
    cv::GaussianBlur(frame, blurred, cv::Size(3,3), 0);
    
    if (currentRequest == VRRequest::GAME_START
        || (currentRequest == VRRequest::MOVE && gameStates.empty()))
    {
        // it will be 3 frame aggregate in the end
        corners = findCorners(blurred);
        // TODO: call game -> start
        
    }
    
    if (corners.empty()) return;
    

    cv::Mat warped;
    warpBoard(corners, frame, warped);

    // calculate whiteness and canny for each field
    cv::Mat wBlur, grey;
    cv::cvtColor(warped, grey, cv::COLOR_BGR2GRAY);
    
    cv::Mat bgr[3];   //destination array
    split(warped, bgr);//split source
    
    grey = bgr[2];
    
    cv::GaussianBlur(warped, wBlur, cv::Size(3,3), 0);
    
    cv::Mat cannys;
    cv::Canny(wBlur, cannys, 20, 100);

    
    for (int i = 0; i < 9; i++) {
        auto p = i * SQS;
        
        cv::line(grey, cv::Point(0, p), cv::Point(BS, p), cv::Scalar(0), 10);
        cv::line(grey, cv::Point(p, 0), cv::Point(p, BS), cv::Scalar(0), 10);
        cv::line(cannys, cv::Point(0, p), cv::Point(BS, p), cv::Scalar(0), 10);
        cv::line(cannys, cv::Point(p, 0), cv::Point(p, BS), cv::Scalar(0), 10);
    }
    
    // aggregate 3 frames with cannys?
    if (currentRequest != VRRequest::NONE) {
        if (agCanny.empty()) {
            agCanny = cannys;
        } else {
            cv::bitwise_or(agCanny, cannys, agCanny);
            agCount++;
        }
    }
    
    if (currentRequest != VRRequest::NONE && agCount > 1)
    {
//        cv::Canny(agCanny, cannys, 20, 100);
        
        GameState gs;
        
        // calculate square values
        cv::Mat sq;
        
        std::string ll = "\n";
        
        std::string avs = "\n";
        
        for (int i = 0; i < 8; i++) {

            for (int j = 0; j < 8; j++) {
                sq = grey(cv::Rect(j * SQS, i * SQS, SQS, SQS));
                gs.whiteness[i * 8 + j].position = i * 8 + j;
                gs.whiteness[i * 8 + j].value = calcWhiteness(sq);
//
                sq = agCanny(cv::Rect(j * SQS, i * SQS, SQS, SQS));
                gs.lines[i * 8 + j].position = i * 8 + j;
                gs.lines[i * 8 + j].value = cv::countNonZero(sq);
                
                char v[8];
                sprintf(v, "%3d ", gs.lines[i * 8 + j].value);
                avs += v;
                
                sprintf(v, "%3d ", gs.whiteness[i * 8 + j].value);
                ll += v;
            }
            ll += "\n";
            avs += "\n";
        }
        
        Log(DBG) << "\n" << ll;
        Log(DBG) << "\n" << avs;
        
        if (currentRequest == VRRequest::GAME_START
            || (currentRequest == VRRequest::MOVE && gameStates.empty()))
        {
            gameStates.clear();
            gameStates.push_back(std::move(gs));
            Log(DBG) << "Game start";
            
            for (int i = 0; i < 16; i++) {
                takenSquares[i] = 2; // black
                takenSquares[63 - i] = 1; // white
            }
            for (int i = 16; i < 32; i++) {
                takenSquares[i] = 0; // none
                takenSquares[63 - i] = 0; // none
            }
            whitesMove = true;
            
            // TODO: call game -> start
            
        } else if (currentRequest == VRRequest::MOVE) {
            
            // find move
            // calculate differences with previous state
            GameState diffs;
            const auto& last = gameStates.back();
            
            for (int i = 0; i < 64; i++) {
                
                diffs.lines[i].position = gs.lines[i].position;
                diffs.whiteness[i].position = gs.whiteness[i].position;
                
                diffs.lines[i].value = abs(gs.lines[i].value - last.lines[i].value);
                diffs.whiteness[i].value = abs(gs.whiteness[i].value - last.whiteness[i].value);
            }
            
            std::sort(diffs.lines, diffs.lines + 64, [](SquareVR& a, SquareVR& b) {
                return a.value > b.value;
            });
            
            std::sort(diffs.whiteness, diffs.whiteness + 64, [](SquareVR& a, SquareVR& b) {
                return a.value > b.value;
            });
            
            // push it to game. if game says it is valid, push it into states
            
            // TODO: should check for castling
            
            // last lines is from where it came
            // going in order of biggest changes
            int from = diffs.lines[63].position;
            for (int i = 0; i < 64; i++) {
                
                auto& ls = last.lines[diffs.lines[i].position];
                auto& cs = gs.lines[diffs.lines[i].position];
                
                Log(DBG) << "Try FR " << ls.value << " " << cs.value;
                
                // white is taken by black on black square?
                // sq should be taken by correct color piece
                // TODO: castling
                if (takenSquares[diffs.lines[i].position] == (whitesMove ? 1 : 2)
                    && cs.value < 150 && diffs.lines[i].value > ls.value * 0.5)
                {
                    from = diffs.lines[i].position;
                    Log(DBG) << "FR " << p(from);
                    break;
                }
            }
            
            int to = -1;
            
            // should start with whiteness changes?
            // TODO: castling
            Log(DBG) << diffs.whiteness[0].value << p(diffs.whiteness[0].position) << " " << diffs.whiteness[1].value << p(diffs.whiteness[1].position) << " " << diffs.whiteness[2].value << p(diffs.whiteness[2].position);
            
            for (int i = 0; i < 64; i++) {
                if (diffs.whiteness[i].position == from) continue;
                
                if (diffs.whiteness[i].value > 35) // should be change
                {
                    to = diffs.whiteness[i].position;
                    Log(DBG) << "To Detected by whiteness";
                } else break;
            }
            
            if (to == -1) { // not detected by whiteness
                
                for (int i = 0; i < 64; i++) {
                    // move should be to free sq
                    // takes should be detected by whiteness detection
                    if (takenSquares[diffs.lines[i].position] == (whitesMove ? 1 : 2)) continue;
                    
                    // is change big enough?
                    // whiteness check can be included
                    if (diffs.lines[i].value > 70) {
                        to = diffs.lines[i].position;
                        break;
                    }
                }
            }
            
            if (from >= 0 && to >= 0) {
                //            char ff = 'A' + (from % 8);
                //            char tf = 'A' + (to % 8);
                //            int fr = 8 - (from / 8);
                //            int tr = 8 - (to / 8);
                            
                            Log(DBG) << gameStates.size() + 1 << " MOVE >> " << p(from) << " -> " << p(to);
                            Log(DBG)
                            << diffs.lines[63].value << p(diffs.lines[63].position) << " "
                            << diffs.lines[62].value << p(diffs.lines[62].position) << " "
                            << diffs.lines[1].value << p(diffs.lines[1].position) << " "
                            << diffs.lines[0].value << p(diffs.lines[0].position);
                            
                            
                            gameStates.push_back(std::move(gs));
                            // first can be to where if absolute change is big enough. otherwise needs to check whiteness
                            
                            takenSquares[from] = 0;
                            takenSquares[to] = (whitesMove ? 1 : 2);
                            
                            whitesMove = !whitesMove;
                
                std::string B;
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if (takenSquares[i * 8 + j] == 2) {
                            B += "B ";
                        } else if (takenSquares[i * 8 + j] == 1) {
                            B += "W ";
                        } else {
                            B += ". ";
                        }
                    }
                    B += "\n";
                }
                Log(DBG) << "\n" << B;
                
            } else {
                Log(ERR) << gameStates.size() + 1 << " -> Can no decide on move!!!";
            }
                

        }
        
        // save everything
        auto no = std::to_string(gameStates.size());
        cv::imwrite("1_" + no + "_canny.png", cannys);
        cv::imwrite("2_" + no + "_orig.png", cframe.frame);
//        cv::imwrite(no + "_bw.png", grey);
        
        currentRequest = VRRequest::NONE;
        agCount = 0;
        agCanny = cv::Mat();
        
    }
    
    app.show("GR", grey);
    app.show("WC", cannys);

}

}
