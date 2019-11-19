#pragma once

#include <array>

#include "App.hpp"
#include "Utils.hpp"

namespace aic
{
struct SquareVR
{
    uint32_t value;
    uint32_t position;
};
struct GameState
{
    SquareVR whiteness[64];
    SquareVR lines[64];
    
    VRRequest request;
};

class Game
{
public:
    Game(App& app);
    ~Game();
    
    void pushFrame(GameState& frame);

private:
    void run();
    
private:
    App& app;

    std::atomic_bool stoped;
    std::thread th;

    ConcurrentQueue<GameState> frames;
};
}
