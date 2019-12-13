#include "Game.hpp"
#include "Chess.hpp"
#include "Log.hpp"

namespace aic
{

bool aic::Game::isMoveValid(aic::Piece piece, char dst_file, int dst_rank)
{
    /* Validate if the destination position does not go beyond board borders. */
    if (dst_file > 'H' || dst_file < 'A' || dst_rank < 1 || dst_rank > 8) {
        Log(DBG) << "Invalid destination position: "
                 << dst_file << " " << dst_rank;
        return false;
    }
    else
    {
        char src_file = piece.getPieceFile();
        int src_rank = piece.getPieceRank();
        
        switch (piece.getPieceType())
        {
            case Piece::Type::PAWN: {
                /* Check cases for both pawn colors. */
                switch (piece.getPieceColor())
                {
                    case Piece::Color::WHITE: {
                        return (dst_rank > src_rank);
                        break;
                    }
                        
                    case Piece::Color::BLACK: {
                        return (dst_rank < src_rank);
                        break;
                    }
                }
                break;
            }
            
            case Piece::Type::KNIGHT: {
                return (calcManhattanDist(src_file, src_rank, dst_file, dst_rank) == 3);
                break;
            }
                
            /* TODO: Implement check for enemy pieces in the move path of below pieces */
            case Piece::Type::BISHOP: {
                return (onDiagonal(src_file, src_rank, dst_file, dst_rank));
                break;
            }
                
            case Piece::Type::ROOK: {
                return (onOrthogonal(src_file, src_rank, dst_file, dst_rank));
                break;
            }
                
            case Piece::Type::QUEEN: {
                return (onDiagonal(src_file, src_rank, dst_file, dst_rank) ||
                            onOrthogonal(src_file, src_rank, dst_file, dst_rank));
                break;
            }
                
            case Piece::Type::KING: {
                return (calcManhattanDist(src_file, src_rank, dst_file, dst_rank) == 1 ||
                        canCastleBasic(src_file, src_rank, dst_file, dst_rank));
                break;
            }
                
        }
    }
}

/*
 x = (a, b) ;y = (c, d) ;
 dst = |a - c| + |b - d|
 */
int aic::Game::calcManhattanDist(char a, int b, char c, int d)
{
     return (int)abs(a - c) + abs(b - d);
}

bool aic::Game::onDiagonal(char a, int b, char c, int d)
{
    return ((int)abs(a - c) == abs(b - d));
}

bool aic::Game::onOrthogonal(char a, int b, char c, int d)
{
    // either rank is equal or the file is equal
    return (a == c || b == d);
}

bool aic::Game::canCastleBasic(char a, int b, char c, int d)
{
    aic::Piece::Color color;
    
    if (b <= 4) { color = aic::Piece::Color::WHITE; } else color = aic::Piece::Color::BLACK;
    
    switch (color)
    {
        case Piece::Color::WHITE: {
            return (b == 1 && d == 1) && (abs(a - c) == 2);
            
            break;
        }
            
        case Piece::Color::BLACK: {
            if (b == 8 && d == 8)
            {
                return (abs(a - c) == 2);
            } else return false;
            break;
        }
    }
}

}
