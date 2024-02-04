#include <SFML/Graphics.hpp>
#include "iostream"
#include "game.h"
#include "main_menu.h"

int main()
{
    
    std::shared_ptr<Main_menu> main_menu = std::make_shared<Main_menu>();
    main_menu->run();
    return 0;
}