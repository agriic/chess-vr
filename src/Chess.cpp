#include "Chess.hpp"
#include <utility>

Piece::Piece(Type t, Piece::Color col, char colum, int lvl) {
    setPieceType(t);
    setPieceColor(col);
    setPieceColumn(colum);
    setPieceLevel(lvl);
}

void Piece::setPieceType(Piece::Type t) {
    type = t;
}

void Piece::setPieceColor(Piece::Color col) {
    color = col;
}

void Piece::setPieceColumn(char colum) {
    column = colum;
}

void Piece::setPieceLevel(int lvl) {
    level = lvl;
}

Board::Board() = default;

void Board::setBoardState(std::vector<Piece> pieces) {
    for (auto piece : pieces) {
        boardState.push_back(piece);
    }
}

void Board::setDefaultBoardState() {
    // populate with pawns for white and black
    for (int i = 'A'; i < 'I'; ++i) {
        boardState.push_back(Piece(Piece::Type::PAWN, Piece::Color::WHITE, (char)i, 2));
        boardState.push_back(Piece(Piece::Type::PAWN, Piece::Color::BLACK, (char)i, 7));
    }
    // TODO: Implement placement of other pieces
}

Board::~Board() = default;
