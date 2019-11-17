//
// Created by Daniils Petrovs on 17/11/2019.
//
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

    explicit Piece(Type t, Color col, char colum, char lvl);

    void setPieceType(Type t);
    void setPieceColor(Color col);
    void setPieceColumn(char colum);
    void setPieceLevel(char lvl);

private:
    Type type{};
    Color color{};
    char column{};
    char level{};
};

class Board {

public:
    std::vector<Piece> boardState;
    Board(const std::vector<Piece>& pieces);

    void setBoardState(std::vector<Piece> pieces);
};


#endif //CHESS_VR_CHESS_HPP
