#include <iostream>

#include "game.hpp"

// TODO:
// Make a themes.json for colours

int main()
{
    Rummage::Game game;

    while (game.isRunning())
    {
        game.update();
        game.render();
    }
}