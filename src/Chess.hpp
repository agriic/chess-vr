#include <vector>

#ifndef CHESS_VR_CHESS_HPP
#define CHESS_VR_CHESS_HPP

namespace aic
{
class Piece {

public:
    enum class Type {
        KING,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN
    };
    enum class Color {
        WHITE,
        BLACK
    };

    explicit Piece(Type type, Color color, char file, int rank);

    /*
     * Set the piece location.
     *
     * @param colum char letter (A-H).
     * @return void.
     */
    void setPiecePos(char file, int rank);

    Piece::Type getPieceType()
    {
        return type;
    }

    Piece::Color getPieceColor()
    {
        return color;
    }

    char getPieceFile()
    {
        return file;
    }

    int getPieceRank()
    {
        return rank;
    }

private:
    Piece::Type type;
    Piece::Color color;
    char file;
    int rank;
};

class Board {

public:
    Board();

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

    ~Board();

private:
    std::vector<Piece> boardState;

};
    
}

#endif //CHESS_VR_CHESS_HPP
