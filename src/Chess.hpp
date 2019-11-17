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

    void setPieceType(Type t);
    void setPieceColor(Color col);
    void setPieceColumn(char colum);
    void setPieceLevel(int lvl);

private:
    Type type{};
    Color color{};
    char column{};
    int level{};
};

class Board {

public:
    Board();
    std::vector<Piece> boardState;

    void setBoardState(std::vector<Piece> pieces);
    void setDefaultBoardState();
    ~Board();
};


#endif //CHESS_VR_CHESS_HPP
