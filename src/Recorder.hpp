#include <string>
#include <fstream>
#include <Game.hpp>

#ifndef CHESS_VR_RECORDER_H
#define CHESS_VR_RECORDER_H

namespace aic
{

class Recorder
{
    struct Move {
        std::string moveNo;
        std::string whitePly;
        std::string blackPly;
    };
    
public:
 
    Recorder();

    ~Recorder();

    /*
     * Write PGN tags in interactive mode from user input.
     *
     * @param void
     * @return void
     */
    void writeTagsInteractive();
    
    void addPly(aic::Piece piece, char dst_file, int dst_rank, bool capture);
    void setTempWhitePly(std::string ply);
    void setTempBlackPly(std::string ply);
    void makeMoveText();
    void dumpMoveText();
    
private:
    
    
    
    int currentMove;
    
    Move tempMove;
    std::ofstream outfile;
    std::vector<Move> moves;
    std::string moveText;
    
    /**
        Append a new move to moveText.
     
        @param move Move struct
     
     */
    void addMove(Move move);
    
};
    
}

#endif //CHESS_VR_RECORDER_H
