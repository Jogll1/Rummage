#include <iostream>

#include "game.hpp"

// TODO:
// Make a themes.json for colours
// Add a red border around tiles that will return to hand when the turn ends?

int main()
{
    Rummage::Game game;

    while (game.isRunning())
    {
        game.update();
        game.render();
    }
}