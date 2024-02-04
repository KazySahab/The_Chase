#pragma once
#include "global_function.h"
#include "sound.h"
#include "text.h"
#include "fstream"

class Main_menu
{
	int select_counter = 0;
	int selected = 0;
	int n_options = 4;
	int menu_high_score;
public:
	bool is_play_triggerred = false;
	std::shared_ptr<Sound>menu_bg_sound = std::make_shared<Sound>("background_1", true, 30);
	std::shared_ptr<Show_text>menu_h_score_text = std::make_shared<Show_text>(sf::Vector2f(w_width/2,w_height/2));
	sf::Font font;
	sf::Text main_menu_selection[4];
	sf::RenderWindow menu_window;
	sf::Event event;

	Main_menu();
	
	void draw_menu();
	void run();
	void load_menu_high_score();
		
};

