//
// Created by Daniils Petrovs on 17/11/2019.
//
#include "Chess.hpp"

#include <utility>

Piece::Piece(Type t, Piece::Color col, char colum, char lvl) {
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

void Piece::setPieceLevel(char lvl) {
    level = lvl;
}

Board::Board(const std::vector<Piece>& pieces) {
    for (auto piece : pieces) {
        boardState.push_back(piece);
    }
}

void Board::setBoardState(std::vector<Piece> pieces) {
    boardState = std::move(pieces);
}
