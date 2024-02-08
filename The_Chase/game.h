#pragma once

#include "entity_manager.h"
#include "fstream"
#include "sound.h"
#include"text.h"
#include"global_function.h"



class Game
{
private:
	sf::Event event;
	sf::Texture background_image;
	sf::RectangleShape background;

	sf::Clock e_spawn_time;
	sf::Clock bullet_time;
	sf::Clock delta_clock;
	sf::Clock life_span;
	sf::Clock p_special_power_time;

	Entity_manager m_entities;
	std::shared_ptr<Entity> player;
	
	int m_current_frame = 0;
	int bullet_no = 35;
	int score =0;
	int high_score = 0;
	int p_e_collision_count = 0;
	int show_bg_image_counter = 0;
	int big_e_spawn_interval = 0;
	float remaining_p_special_power=0;

	float delta_time = 0;
	float e_spawn_interval = 3.5f;
	
	void move_entity();
	void handle_input(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
	void collision();
	void check_bullet_health();
	void spawn_enemy();
	void spawn_player();
	void spawn_bullet(std::shared_ptr<Entity>player, const Vec2& target);
	void load_special_power();

	void load_high_score();
	void save_high_score();

	void change_level_bg_image();

	bool is_game_paused = false;
	bool is_game_exit = false;

	std::shared_ptr<Sound> background_sound_1 = std::make_shared<Sound>("background_1", true, 30);
	std::shared_ptr<Sound> background_sound_2 = std::make_shared<Sound>("background_2", true, 45);
	std::shared_ptr<Sound> bullet_sound = std::make_shared<Sound>("bullet", false, 70);
	std::shared_ptr<Sound> big_e_spawn_sound = std::make_shared<Sound>(false, 60);
	std::shared_ptr<Sound> big_e_dead_sound = std::make_shared<Sound>("big_e_dead", false, 60);
	std::shared_ptr<Sound> collision_sound = std::make_shared<Sound>("collision", false, 30);
	std::shared_ptr<Sound> player_dead_sound = std::make_shared<Sound>("player_dead", false, 70);

	std::shared_ptr<Show_text> score_text = std::make_shared<Show_text>(sf::Vector2f(30, 10));
	std::shared_ptr<Show_text> rem_life_text = std::make_shared<Show_text>(sf::Vector2f(30, 85));
	std::shared_ptr<Show_text> bullet_num_text = std::make_shared<Show_text>(sf::Vector2f(w_width - 300, 10));
	std::shared_ptr<Show_text> bullet_reload_time_text = std::make_shared<Show_text>(sf::Vector2f(w_width - 300, 50));
	std::shared_ptr<Show_text> difficulty_level_text = std::make_shared<Show_text>(sf::Vector2f(w_width / 2-100, 10));
	std::shared_ptr<Show_text> game_over_text = std::make_shared<Show_text>(sf::Vector2f(w_width / 2-80, w_height / 2));
	std::shared_ptr<Show_text> p_special_power_text = std::make_shared<Show_text>(sf::Vector2f(w_width / 2 - 80, 20));
	std::shared_ptr<Show_text> sp_activated_text = std::make_shared<Show_text>(sf::Vector2f(w_width / 2, 85));

public:
	Game();
	void run(sf::RenderWindow& window);
	~Game() {};
};




