#pragma once
#include "random"
#include "entity_manager.h"
#include "fstream"
#include "sound.h"
#include"text.h"

const float w_width = sf::VideoMode::getDesktopMode().width;
const float w_height = sf::VideoMode::getDesktopMode().height;

class Game
{
private:
	sf::RenderWindow window;
	sf::Event event;
	sf::Texture background_image;
	sf::RectangleShape background;

	sf::Clock e_spawn_time;
	sf::Clock bullet_time;
	sf::Clock delta_clock;
	sf::Clock life_span;

	Entity_manager m_entities;
	std::shared_ptr<Entity> player;
	
	int m_current_frame = 0;
	int bullet_no = 30;
	int score = 0;
	int high_score = 0;
	int p_e_collision_count = 0;
	int big_e_spawn_interval = 0;


	float delta_time = 0;
	float e_spawn_interval = 3.0f;
	

	void move_entity();
	void handle_input();
	void render();
	void collision();
	void check_bullet_health();
	void spawn_enemy();
	void spawn_player();
	void spawn_bullet(std::shared_ptr<Entity>player, const Vec2& target);
	
	void load_high_score();
	void save_high_score();

	bool is_game_paused = false;
	bool is_game_exit = false;

	std::shared_ptr<Sound> background_sound_1 = std::make_shared<Sound>("background_1", true, 20);
	std::shared_ptr<Sound> background_sound_2 = std::make_shared<Sound>("background_2", true, 30);
	std::shared_ptr<Sound> bullet_sound = std::make_shared<Sound>("witch", true, 80);
	
	std::shared_ptr<Show_text> score_text = std::make_shared<Show_text>(sf::Vector2f(30, 10));
	std::shared_ptr<Show_text> rem_life_text = std::make_shared<Show_text>(sf::Vector2f(30, 85));
	std::shared_ptr<Show_text> bullet_num = std::make_shared<Show_text>(sf::Vector2f(w_width - 300, 10));
	std::shared_ptr<Show_text> bullet_reload_time = std::make_shared<Show_text>(sf::Vector2f(w_width - 300, 50));
public:
	Game();
	void run();
	~Game() {};

	
	
};
float get_random(float min, float max);
std::string get_enemy_image();

