#include "Recorder.hpp"
#include <fstream>
#include <time.h>
#include <chrono>
#include <string>
#include "Log.hpp"
#include "Game.hpp"

namespace aic
{

aic::Recorder::Recorder()
{
    outfile.open("game.pgn");
    aic::Log(aic::DBG) << "Opened file for writing.";
    currentMove = 1;
    tempMove.whitePly = std::string(NULL);
    tempMove.blackPly = std::string(NULL);
}

void aic::Recorder::writeTagsInteractive()
{
    char data[100];
    Log(DBG) << "Please enter event name: ";
    std::cin >> data;
    outfile << "[Event \""
            << data
            << "\"]" << std::endl;

    Log(DBG) << "Please enter city name:";
    std::cin >> data;
    outfile << "[Site \"" << data << ", ";
    Log(DBG) << "Please enter region name:";
    std::cin >> data;
    outfile << data << " ";
    Log(DBG) << "Please center country code:";
    std::cin >> data;
    outfile << data << "\"]" << std::endl;

    time_t t = time(NULL);
    tm* timePtr = localtime(&t);

    int year = (timePtr->tm_year)+1900;
    int month = (timePtr->tm_mon)+1;
    int day = (timePtr->tm_mday);

    outfile << "[Date \"" << year << "." << month << ".";
    if (day > 9) outfile << day; else outfile << "0" << day;
    outfile << "\"]" << std::endl;

    outfile << "[Round \"29\"]" << std::endl;

    Log(DBG) << "Please enter WHITE player's name in format: Surname, Name";
    std::cin >> data;
    outfile << "[White \"" << data << "\"]" << std::endl;

    Log(DBG) << "Please enter BLACK player's name in format: Surname, Name";
    std::cin >> data;
    outfile << "[Black \"" << data << "\"]" << std::endl;

    Log(DBG) << "Please enter round result:";
    std::cin >> data;
    outfile << "[Result \"" << data << "\"]" << std::endl;

    outfile << std::endl; // add a new line before writing moveText
}

void aic::Recorder::addMove(Move move)
{
    move.moveNo = std::to_string(currentMove) + ".";
    moves.push_back(move); // push completed move to vector (both plys are filled)
    ++currentMove;
}

void aic::Recorder::addPly(aic::Piece piece, char dst_file, int dst_rank, bool capture)
{
    std::string pc; // piece type as single char string
    
    char src_file = piece.getFile();
    int src_rank = piece.getRank();
    
    if (aic::Game::canCastleBasic(src_file, src_rank, dst_file, dst_rank))
    {
        std::string castlePly;
        if (dst_file == 'C') { castlePly = "O-O-O"; // queenside
        } else castlePly = "O-O"; // kingside
        
        if (piece.getColor() == aic::Piece::Color::WHITE)
        {
            setTempWhitePly(castlePly);
        } else {
            setTempBlackPly(castlePly);
            addMove(tempMove);
            ++currentMove;
        }
        
    } else {
        switch (piece.getType())
            {
                case Piece::Type::KNIGHT: {
                    pc = "N";
                    break;
                }
                case Piece::Type::BISHOP: {
                    pc = "B";
                    break;
                }
                case Piece::Type::ROOK: {
                    pc = "R";
                    break;
                }
                case Piece::Type::QUEEN: {
                    pc = "Q";
                    break;
                }
                default:
                    break;
            }
        
        std::string ply(pc);
        if (capture) ply += "x";
        
        ply += tolower(dst_file);
        ply += std::to_string(dst_rank);
        
        if (piece.getColor() == aic::Piece::Color::WHITE)
        {
            setTempWhitePly(ply);
        } else {
            setTempBlackPly(ply);
            addMove(tempMove);
            ++currentMove;
        }
    }
}

void aic::Recorder::setTempWhitePly(std::string ply)
{
    tempMove.whitePly = ply;
}

void aic::Recorder::setTempBlackPly(std::string ply)
{
    tempMove.blackPly = ply;
}

void aic::Recorder::makeMoveText()
{
    for (auto & move : moves) {
        moveText += move.moveNo + " " + move.whitePly + " " + move.blackPly + " ";
    }
}

void aic::Recorder::dumpMoveText()
{
    outfile << moveText; // writes single movetext line to open file
}


aic::Recorder::~Recorder()
{
    outfile.close();
    aic::Log(aic::DBG) << "File closed";
}

}
