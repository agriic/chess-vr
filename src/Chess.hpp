#include <vector>

#ifndef CHESS_VR_CHESS_HPP
#define CHESS_VR_CHESS_HPP


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

    explicit Piece(Type t, Color col, char colum, int lvl);

    /*
     * Set the piece Type.
     *
     * @param t Piece Type (KING, QUEEN ...).
     * @return void.
     */
    void setPieceType(Type t);

    /*
     * Set the piece Color.
     *
     * @param col Piece Color (WHITE | BLACK).
     * @return void.
     */
    void setPieceColor(Color col);

    /*
     * Set the piece column location.
     *
     * @param colum char letter (A-H).
     * @return void.
     */
    void setPieceColumn(char colum);

    /*
     * Set the piece level location.
     *
     * @param lvl Level (row) as int (1-8).
     * @return void.
     */
    void setPieceLevel(int lvl);

    Piece::Type getPieceType()
    {
        return this->type;
    }

    Piece::Color getPieceColor()
    {
        return this->color;
    }

    char getPieceCol()
    {
        return this->column;
    }

    int getPieceLvl()
    {
        return this->level;
    }

private:
    Piece::Type type;
    Piece::Color color;
    char column;
    int level;
};

class Board {

public:
    Board();

    /*
     * Set the board state from collection of pieces.
     *
     * @param pieces vector of Pieces
     * @return void.
     */
    void setBoardState(std::vector<Piece> pieces);

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


#endif //CHESS_VR_CHESS_HPP
