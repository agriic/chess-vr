#include "Chess.hpp"
#include <utility>

namespace aic
{

aic::Board::Board() = default;

aic::Piece::Piece(Type t, Piece::Color col, char file, int rank)
{
    type = t;
    color = col;
    this->file = file;
    this->rank = rank;
}


void aic::Piece::setPiecePos(char file, int rank)
{
    this->file = file;
    this->rank = rank;
}

void aic::Board::addPiece(Piece piece)
{
    boardState.push_back(piece);
}

void aic::Board::addPieces(const std::vector<Piece>& pieces)
{
    boardState.insert(boardState.end(), pieces.begin(), pieces.end());
}

void aic::Board::setDefaultBoardState()
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
    addPiece(Piece(Piece::Type::ROOK, Piece::Color::BLACK, 'A', 8));
    addPiece(Piece(Piece::Type::ROOK, Piece::Color::BLACK, 'H', 8));

    addPiece(Piece(Piece::Type::KNIGHT, Piece::Color::BLACK, 'B', 8));
    addPiece(Piece(Piece::Type::KNIGHT, Piece::Color::BLACK, 'G', 8));

    addPiece(Piece(Piece::Type::BISHOP, Piece::Color::BLACK, 'C', 8));
    addPiece(Piece(Piece::Type::BISHOP, Piece::Color::BLACK, 'F', 8));

    addPiece(Piece(Piece::Type::QUEEN, Piece::Color::BLACK, 'D', 8));
    addPiece(Piece(Piece::Type::KING, Piece::Color::BLACK, 'E', 8));
}

int aic::Board::getChessBoardPieceCount()
{
    return boardState.size();
}

aic::Piece aic::Board::getPieceByIndex(int index)
{
    return boardState.at(index);
}

std::vector<aic::Piece> aic::Board::getBoardState()
{
    return boardState;
}

aic::Board::~Board() = default;

}

