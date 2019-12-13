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
    
    void addPly(aic::Piece piece, char dst_file, int dst_rank, bool capture); //pushes a ply to a move
    void setTempWhitePly(std::string ply);
    void setTempBlackPly(std::string ply);
    
    void makeMoveText();
    void dumpMoveText(); // serializes the built moveText to the output file stream
    
private:
    int currentMove;
    
    Move tempMove; // temporary Move used for filling with ply values, pushed to moves when both plys are filled
    std::ofstream outfile; // output file stream for writing tags and moveText
    std::vector<Move> moves; // vector for storing moves
    std::string moveText; // string of entire complete moveText for the game
    
    /**
        Append a new move to moveText.
     
        @param move Move struct
     
     */
    void addMove(Move move);
    
};
    
}

#endif //CHESS_VR_RECORDER_H
