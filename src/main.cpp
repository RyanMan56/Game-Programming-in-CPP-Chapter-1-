#include <iostream>
#include "game.h"

int main()
{
    Game game;
    bool success = game.initialise();
    std::cout << "Initialised\n";
    if (success)
    {
        game.runLoop();
    }
    game.shutdown();
    return 0;
}