#pragma once
#include "random"
#include "entity_manager.h"

class Game
{
private:
	sf::RenderWindow window;
	sf::Event event;
	sf::Font font;
	sf::Text score_text;
	Entity_manager m_entities;
	std::shared_ptr<Entity> player;
	sf::Clock delta_clock;
	sf::Clock life_span;
	sf::Text bullet_num;
	sf::Text bullet_reload_time;
	sf::Text help_text;
	sf::Clock bullet_time;

	int bullet_no = 30;
	float delta_time = 0;
	int score = 0;
	int m_current_frame = 0;

	sf::Clock e_spawn_time;
	float e_spawn_interval = 2.0f;

	void move_entity();
	void handle_input();
	void render();
	void collision();
	void life_spawn();
	void spawn_enemy();
	void spawn_player();
	void spawn_bullet(std::shared_ptr<Entity>player, const Vec2& target);

	bool is_game_paused = false;
	bool is_game_exit = false;
public:
	Game();
	void run();
	~Game() {};
};
float get_random(float min, float max);


