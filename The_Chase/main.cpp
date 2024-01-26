#include <SFML/Graphics.hpp>
#include "iostream"
#include "game.h"

int main()
{
    try {
        Game game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}