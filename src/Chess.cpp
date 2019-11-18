#include "Chess.hpp"
#include <utility>

Board::Board() = default;

Piece::Piece(Type t, Piece::Color col, char colum, int lvl)
{
    this->type = t;
    this->color = col;
    this->column = colum;
    this->level = lvl;
}

void Piece::setPieceType(Piece::Type t)
{
    type = t;
}

void Piece::setPieceColor(Piece::Color col)
{
    color = col;
}

void Piece::setPieceColumn(char colum)
{
    column = colum;
}

void Piece::setPieceLevel(int lvl)
{
    level = lvl;
}

void Board::addPiece(Piece piece)
{
    boardState.push_back(piece);
}

void Board::addPieces(const std::vector<Piece>& pieces)
{
    for (auto piece : pieces) {
        boardState.push_back(piece);
    }
}

void Board::setDefaultBoardState()
{
    // Populate with pawns for white and black.
    for (int i = 'A'; i < 'I'; ++i)
    {
        addPiece(Piece(Piece::Type::PAWN, Piece::Color::WHITE, (char)i, 2));
        addPiece(Piece(Piece::Type::PAWN, Piece::Color::BLACK, (char)i, 7));
    }
    /* White pieces */
    addPiece(Piece(Piece::Type::ROOK, Piece::Color::WHITE, 'A', 1));
    addPiece(Piece(Piece::Type::ROOK, Piece::Color::WHITE, 'H', 1));

    addPiece(Piece(Piece::Type::KNIGHT, Piece::Color::WHITE, 'B', 1));
    addPiece(Piece(Piece::Type::KNIGHT, Piece::Color::WHITE, 'G', 1));

    addPiece(Piece(Piece::Type::BISHOP, Piece::Color::WHITE, 'C', 1));
    addPiece(Piece(Piece::Type::BISHOP, Piece::Color::WHITE, 'F', 1));

    addPiece(Piece(Piece::Type::QUEEN, Piece::Color::WHITE, 'D', 1));
    addPiece(Piece(Piece::Type::KING, Piece::Color::WHITE, 'E', 1));

    /* Black pieces */
    addPiece(Piece(Piece::Type::ROOK, Piece::Color::BLACK, 'A', 1));
    addPiece(Piece(Piece::Type::ROOK, Piece::Color::BLACK, 'H', 1));

    addPiece(Piece(Piece::Type::KNIGHT, Piece::Color::BLACK, 'B', 1));
    addPiece(Piece(Piece::Type::KNIGHT, Piece::Color::BLACK, 'G', 1));

    addPiece(Piece(Piece::Type::BISHOP, Piece::Color::BLACK, 'C', 1));
    addPiece(Piece(Piece::Type::BISHOP, Piece::Color::BLACK, 'F', 1));

    addPiece(Piece(Piece::Type::QUEEN, Piece::Color::BLACK, 'D', 1));
    addPiece(Piece(Piece::Type::KING, Piece::Color::BLACK, 'E', 1));
}

int Board::getChessBoardPieceCount()
{
    return boardState.size();
}

Piece Board::getPieceByIndex(int index)
{
    return boardState.at(index);
}

std::vector<Piece> Board::getBoardState()
{
    return boardState;
}

Board::~Board() = default;
