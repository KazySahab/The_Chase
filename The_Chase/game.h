#pragma once
#include "random"
#include "entity_manager.h"

class Game
{
private:
	sf::RenderWindow window;
	sf::Event event;
	sf::Font font;
	sf::Texture background_image;
	sf::RectangleShape background;

	sf::Text score_text;
	sf::Text bullet_num;
	sf::Text bullet_reload_time;
	sf::Text help_text;
	
	sf::Clock e_spawn_time;
	sf::Clock bullet_time;
	sf::Clock delta_clock;
	sf::Clock life_span;

	Entity_manager m_entities;
	std::shared_ptr<Entity> player;
	
	int m_current_frame = 0;
	int bullet_no = 30;
	int score = 0;
	int p_e_collision_count = 0;
	int big_e_spawn_interval = 0;
	int big_e_health = 5;


	float delta_time = 0;
	float e_spawn_interval = 3.0f;
	

	void move_entity();
	void handle_input();
	void render();
	void collision();
	void life_spawn();
	void spawn_enemy();
	void spawn_player();
	void spawn_bullet(std::shared_ptr<Entity>player, const Vec2& target);
	void additional_game_functions();

	bool is_game_paused = false;
	bool is_game_exit = false;
public:
	Game();
	void run();
	~Game() {};
};
float get_random(float min, float max);
std::string get_enemy_image();

