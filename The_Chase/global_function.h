#pragma once
#include"iostream"
#include"SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "random"

const float w_width = sf::VideoMode::getDesktopMode().width;
const float w_height = sf::VideoMode::getDesktopMode().height;



float get_random(float min, float max);
std::string get_big_e_spawn_sound();
std::string get_enemy_image();