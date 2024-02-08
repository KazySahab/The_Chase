#pragma once
#include "global_function.h"
#include "sound.h"
#include "text.h"
#include "fstream"
#include "game.h"

class Main_menu
{
	int select_counter = 0;
	int selected = 0;
	int n_options = 5;
	int menu_high_score=0;

public:

	bool is_play_triggerred = false;
	bool is_instruction_triggerrred = false;
	bool is_high_score_triggerred = false;
	bool is_about_us_triggerred = false;

	std::shared_ptr<Sound>menu_bg_sound = std::make_shared<Sound>("background_1", true, 40);
	std::shared_ptr<Sound>scary_welcome = std::make_shared<Sound>("welcome", false, 50);
	std::shared_ptr<Sound>menu_scroll = std::make_shared<Sound>("menu_up_down", false, 60);
	std::shared_ptr<Sound>menu_select = std::make_shared<Sound>("menu_open", false, 60);
	std::shared_ptr<Show_text>menu_h_score_text = std::make_shared<Show_text>(sf::Vector2f(w_width/2.5,w_height/2.3));

	sf::Font font;
	sf::Text main_menu_selection[5];
	sf::RenderWindow window;
	sf::Event event;
	sf::Texture menu_bg_image;
	sf::RectangleShape menu_background;
	sf::Image icon;


	Main_menu();

	void draw_menu();
	void run();
	void load_menu_high_score();
	void load_scene();
		
};

