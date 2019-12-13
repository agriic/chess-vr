#pragma once

#include <array>

#include "App.hpp"
#include "Utils.hpp"
#include "Chess.hpp"

namespace aic
{

class Game
{
public:
    Game(App& app);
    ~Game();
    
    void pushFrame(GameState& frame);
    /* Returns the validity of a move */
    bool isMoveValid(aic::Piece piece, char file, int rank);
    
    /**
     
     Find out if piece can castle by moving to destination position.
    
     @param a Source file.
     @param b Source rank.
     @param c Destination file.
     @param d Destination rank.
     @return Boolean if piece can castle, false otherwise.
       
     */
    static bool canCastleBasic(char a, int b, char c, int d);
    
    /**
    Calculates the manhattan distance between two chess board positions.
     
    @param a Source file.
    @param b Source rank.
    @param c Destination file.
    @param d Destination rank.
    @return Manhattan distance.
     */
    int calcManhattanDist(char a, int b, char c, int d);

private:

    
    /**
     Find out if two positions on the board are on the same diagonal.
     
     @param a Source file.
     @param b Source rank.
     @param c Destination file.
     @param d Destination rank.
     @return Boolean true if on diagonal, false otherwise.
     
     */
    bool onDiagonal(char a, int b, char c, int d);
    
    /**
     Find out if two positions are on one of two possible orthogonals (horizontal or vertical).
  
     @param a Source file.
     @param b Source rank.
     @param c Destination file.
     @param d Destination rank.
     @return Boolean true if on orthogonal, false otherwise.
     
     */
    bool onOrthogonal(char a, int b, char c, int d);
    
    void run();
    
private:
    App& app;
};

}
