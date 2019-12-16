#include <CoreFoundation/CoreFoundation.h>
#include <utility>

#include "Board.hpp"
#include "Utils.hpp"

#include "Log.hpp"

aic::Piece::Piece()
{}

aic::Piece::Piece(Type _type, Color _color, char _file, int _rank)
    : type(_type), color(_color), file(_file), rank(_rank)
{}


void aic::Piece::setPiecePos(char file, int rank)
{
    this->file = file;
    this->rank = rank;
}



aic::Board::Board()
{
    // Load resources
    CFURLRef appUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("BB.png"), NULL, NULL);
    CFStringRef filePathRef = CFURLCopyPath(appUrlRef);
    
    std::string filePath = CFStringGetCStringPtr(filePathRef, kCFStringEncodingUTF8);
    
    // Release references
    CFRelease(filePathRef);
    CFRelease(appUrlRef);
    
    std::vector<std::string> images = {
        "", "WK.png", "WQ.png", "WR.png", "WB.png", "WN.png", "WP.png",
        "", "BK.png", "BQ.png", "BR.png", "BB.png", "BN.png", "BP.png"
        };
    
//    pieceImages.resize(14);
    for (int i = 0 ; i < images.size(); i++) {
        auto imgPath = replaceAll(filePath, "BB.png", images[i]);
        cv::Mat img = cv::imread(imgPath, cv::IMREAD_UNCHANGED);
        
        Log(DBG) << "LI " << i << " = " << images[i];
        
        pieceImages.push_back(img);
    }
    
    Log(DBG) << pieceImages.size();
    
    setDefaultBoardState();
}

aic::Board::~Board()
{
//    pieceImages.clear();
//    pieces.clear();
}

void aic::Board::addPiece(Piece piece)
{
    pieces.push_back(piece);
    // calculate board pos
    int pos = ((piece.getRank()-1) * 8 + piece.getFile() - 'A');
    board[pos] = piece;
}

void aic::Board::addPieces(const std::vector<Piece>& newPieces)
{
    pieces.insert(pieces.end(), newPieces.begin(), newPieces.end());
}

void aic::Board::setDefaultBoardState()
{
    pieces.clear();
    for (int i = 0; i < 64; i++) {
        board[i] = Piece();
    }
    
    // Populate with pawns for white and black.
    for (int i = 'A'; i < 'I'; ++i)
    {
        addPiece({Piece::PAWN, Piece::WHITE, (char)i, 2});
        addPiece({Piece::PAWN, Piece::BLACK, (char)i, 7});
    }
    
    /* White pieces */
    addPiece({Piece::ROOK, Piece::WHITE, 'A', 1});
    addPiece({Piece::ROOK, Piece::WHITE, 'H', 1});

    addPiece({Piece::KNIGHT, Piece::WHITE, 'B', 1});
    addPiece({Piece::KNIGHT, Piece::WHITE, 'G', 1});

    addPiece({Piece::BISHOP, Piece::WHITE, 'C', 1});
    addPiece({Piece::BISHOP, Piece::WHITE, 'F', 1});

    addPiece({Piece::QUEEN, Piece::WHITE, 'D', 1});
    addPiece({Piece::KING, Piece::WHITE, 'E', 1});

    /* Black pieces */
    addPiece({Piece::ROOK, Piece::BLACK, 'A', 8});
    addPiece({Piece::ROOK, Piece::BLACK, 'H', 8});

    addPiece({Piece::KNIGHT, Piece::BLACK, 'B', 8});
    addPiece({Piece::KNIGHT, Piece::BLACK, 'G', 8});

    addPiece({Piece::BISHOP, Piece::BLACK, 'C', 8});
    addPiece({Piece::BISHOP, Piece::BLACK, 'F', 8});

    addPiece({Piece::QUEEN, Piece::BLACK, 'D', 8});
    addPiece({Piece::KING, Piece::BLACK, 'E', 8});
}

int aic::Board::getChessBoardPieceCount()
{
    return pieces.size();
}

std::vector<aic::Piece> aic::Board::getBoardState()
{
    return pieces;
}


void overlayImage(cv::Mat* src, cv::Mat* overlay, const cv::Point& location)
{
    for (int y = std::max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;

        if (fY >= overlay->rows)
            break;

        for (int x = std::max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;

            if (fX >= overlay->cols)
                break;

            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}

cv::Mat aic::Board::getStateDrawing()
{
    Log(DBG) << "-->" << pieceImages.size();
    
    const int sq = 64;
    
    auto darkCol = cv::Scalar(102, 136, 180);
    auto lightCol = cv::Scalar(183, 217, 240);
    
    cv::Mat draw = cv::Mat::zeros(sq * 8, sq * 8, CV_8UC3);
    
    cv::rectangle(draw, cv::Point(0,0), cv::Point(sq * 8 - 1, sq * 8 -1), darkCol, -1);
    for (int i = 0; i < 64; i += 2) {
        auto p = cv::Point((i%8 + (i/8)%2) * sq, (i/8) * sq);
        cv::rectangle(draw, p, {p.x + sq -1, p.y + sq -1}, lightCol, -1);
    }
    
    
    // draw pieces over
    for (int i = 0; i < 64; i++) {
        
        auto& p = board[i];
        
        if (p.getType() == Piece::NO_TYPE || p.getColor() == Piece::NO_COLOR) continue;
        
        auto pos = cv::Point((p.getFile() - 'A') * sq, (8 - p.getRank()) * sq);
        
        // get type
        int type = (((int)p.getColor()) - 1) * 7 + (int)p.getType();
        
        overlayImage(&draw, &pieceImages[type], pos);
    }
    
    return draw;
}
