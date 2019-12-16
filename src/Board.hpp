#pragma once

#include <vector>
#include <map>
#include <opencv2/core.hpp>

namespace aic
{
class Piece {

public:
    enum Type {
        NO_TYPE,
        KING,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN
    };
    enum Color {
        NO_COLOR,
        WHITE,
        BLACK
    };

    Piece();
    
    Piece(Type type, Color color, char file, int rank);

    /*
     * Set the piece location.
     *
     * @param colum char letter (A-H).
     * @return void.
     */
    void setPiecePos(char file, int rank);

    Piece::Type getType() { return type; }

    Piece::Color getColor() { return color; }

    char getFile() { return file; }

    int getRank() { return rank; }

private:
    Piece::Type type = NO_TYPE;
    Piece::Color color = NO_COLOR;
    char file = '_';
    int rank = 0;
};

class Board {

public:
    Board();
    ~Board();
    
    
    void addPiece(Piece piece);
    /*
     * Set the board state from collection of pieces.
     *
     * @param pieces vector of Pieces
     * @return void.
     */
    void addPieces(const std::vector<Piece>& pieces);

    /*
     * Set default board state as start of a normal game.
     *
     * @param none.
     * @return void.
     */
    void setDefaultBoardState();

    /*
     * Get number of pieces currently on board.
     *
     * @param none
     * @return int
     */
    int getChessBoardPieceCount();

    /*
     * Get a Piece from the board by index.
     *
     * @param index Index of the piece in the board state vector.
     * @return Piece
     */
    Piece getPieceByIndex(int index);

    std::vector<Piece> getBoardState();
    
    cv::Mat getStateDrawing();
    

private:
    
    std::vector<Piece> pieces;
    
    // From A1 to H8
    Piece board[64];
    
    std::vector<cv::Mat> pieceImages;

};
    
}

