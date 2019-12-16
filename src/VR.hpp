#pragma once

#include "ConcurrentQueue.hpp"
#include "Utils.hpp"
#include "Board.hpp"

namespace aic
{
class App;

struct SquareVR
{
    int value;
    int position;
};
struct GameState
{
    SquareVR whiteness[64];
    SquareVR lines[64];
    
    VRRequest request;
};

class VR
{
    struct Line {
        cv::Vec2f detected;
        float angle;
        cv::Point start;
    };

public:
    VR(App& app);
    ~VR();

    void pushFrame(CapturedFrame& frame);

public:

    const int SQS = 64; // square size
    const int BS = SQS * 8; // board size

private:
    void run();
    void processFrame(CapturedFrame& frame);

    std::vector<cv::Point2f> findCorners(cv::Mat& frame);

    static bool findOrthogonals(std::vector<cv::Vec2f>& lines);

    void warpBoard(std::vector<cv::Point2f>& croners, cv::Mat& source, cv::Mat& out, bool rotationSearch = false);

    void findRotation(cv::Mat& warpedImage);

    void boardCell(cv::Mat& src, cv::Mat& dst, char column, int lvl);

    static bool cellHasPiece(cv::Mat &src, int thresh);

private:
    App& app;

    std::atomic_bool stoped;
    std::thread th;

    ConcurrentQueue<CapturedFrame> frames;

    int rotation = -1;
    
    std::vector<GameState> gameStates;
    
    cv::Mat agCanny;
    uint32_t agCount = 0;
    VRRequest currentRequest = VRRequest::NONE;
    
    std::vector<cv::Point2f> corners;
    
    int takenSquares[64];
    bool whitesMove = true;
    
    Board board;
};
}
