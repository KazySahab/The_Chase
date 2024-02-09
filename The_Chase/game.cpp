#include "game.h"
#include "global_function.h"

int game_pause_counter = 0;

Game::Game()
{
	show_bg_image_counter++;
	if (!background_image.loadFromFile("images/background_image.jpg"))
	{
		std::cout << "background Load failed";
	}
	background.setSize(sf::Vector2f(w_width, w_height));
	background.setTexture(&background_image);
	difficulty_level_text->write.setString("Level : Beginner \nNext level at : 80 " );
	sp_activated_text->write.setCharacterSize(30);
	game_over_text->write.setString("GAME OVER!");
	game_over_text->write.setCharacterSize(60);
	load_high_score();
}


void Game::run(sf::RenderWindow &window)
{

		window.setTitle("The Chase");
		spawn_player();
		background_sound_1->sound.play();
		background_sound_2->sound.play();

		while (window.isOpen())
		{

			sf::Time frame_time = delta_clock.restart();
			delta_time = frame_time.asSeconds();
			handle_input(window);
			m_entities.update();
			score_text->write.setString(" High Score : " + std::to_string(high_score) + "\n Score : " + std::to_string(score));
			rem_life_text->write.setString(" Life :" + std::to_string(3 - p_e_collision_count));
			bullet_reload_time_text->write.setString("Reload Time (30s) : " + std::to_string((int)bullet_time.getElapsedTime().asSeconds()) + "\n Special Power : " + std::to_string((int)remaining_p_special_power));
			bullet_num_text->write.setString("Remaining Bullets : " + std::to_string(bullet_no));
			sp_activated_text->write.setString(std::to_string(5-(int)p_special_power_time.getElapsedTime().asSeconds()));
			

			if (!is_game_paused)
			{
				move_entity();
				collision();
				render(window);
				if (e_spawn_time.getElapsedTime().asSeconds() > e_spawn_interval)
				{
					big_e_spawn_interval++;
					spawn_enemy();
					e_spawn_time.restart();
				}
				if (bullet_time.getElapsedTime().asSeconds() > 30)
				{
					bullet_no += 25;
					remaining_p_special_power+=0.5;
					bullet_time.restart();
				}
				if (background_sound_1->is_paused && background_sound_2->is_paused)
				{
					background_sound_1->sound.play();
					background_sound_1->is_paused = false;
					background_sound_2->sound.play();
					background_sound_2->is_paused = false;
				}
			}
			if (is_game_paused)
			{
				bullet_time.restart();
				background_sound_1->sound.pause();
				background_sound_1->is_paused = true;
				background_sound_2->sound.pause();
				background_sound_2->is_paused = true;
			}
			load_special_power();
			change_level_bg_image();
			check_bullet_health();
			save_high_score();
			m_current_frame++;

			if (is_game_exit)
			{
				background_sound_1->sound.stop();
				background_sound_2->sound.stop();
				if(p_e_collision_count>2) sf::sleep(sf::seconds(3));
				window.setTitle("Main Menu");
				break;
			}

		}
		
	
}

void Game::handle_input(sf::RenderWindow& window)
{

	while (window.pollEvent(event)) {


		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
			


		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::W:
					player->input->up = true;
					break;
				
				case sf::Keyboard::A:
					player->input->left = true;
					break;
				
				case sf::Keyboard::S:
					player->input->down = true;
					break;
				
				case sf::Keyboard::D:
					player->input->right = true;
					break;
				
				case sf::Keyboard::P:
					if (game_pause_counter % 2 == 0) this->is_game_paused = true;
					else this->is_game_paused = false;
					game_pause_counter++;
					break;

				case sf::Keyboard::Escape:
					this->is_game_exit = true;
					break;
			}
		}
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::W:
					player->input->up = false;
					break;

				case sf::Keyboard::A:
					player->input->left = false;
					break;

				case sf::Keyboard::S:
					player->input->down = false;
					break;
				
				case sf::Keyboard::D:
					player->input->right = false;
					break;
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawn_bullet(player, Vec2(event.mouseButton.x, event.mouseButton.y));
				
				if (!player->input->special_weapon)
				{
					if (bullet_no > 0)
					{
						bullet_no -= 1;
					}

				}
				
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (remaining_p_special_power > 0.5)
				{
				
					player->shape->circle.setFillColor(sf::Color::Red);
					p_special_power_time.restart();
					player->input->special_weapon = true;
					remaining_p_special_power--;
					
				}
				
			}
		}

		//joystick Input

		if (sf::Joystick::isConnected(0))
		{
			if (event.type == sf::Event::JoystickMoved)
			{
				float j_move_x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
				float j_move_y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
				float move_limit = 20.0f;

				//move_player
				if (j_move_x < -move_limit)
				{
					player->input->left = true;
				}
				if (j_move_x > move_limit)
				{
					player->input->right = true;
				}
				if (j_move_y < -move_limit)
				{
					player->input->up = true;
				}
				if (j_move_y > move_limit)
				{
					player->input->down = true;
				}

				//reset move_player

				if (j_move_x > -move_limit)
				{
					player->input->left = false;
				}
				if (j_move_x < move_limit)
				{
					player->input->right = false;
				}
				if (j_move_y > -move_limit)
				{
					player->input->up = false;
				}
				if (j_move_y < move_limit)
				{
					player->input->down = false;
				}
			}

			if (event.type == sf::Event::JoystickButtonPressed)
			{
				if (sf::Joystick::isButtonPressed(0, 0))
				{
					spawn_bullet(player, Vec2(player->transform->pos.x, player->transform->pos.y + 100));
				}
				else if (sf::Joystick::isButtonPressed(0, 1))
				{
					spawn_bullet(player, Vec2(player->transform->pos.x + 100, player->transform->pos.y));
				}
				else if (sf::Joystick::isButtonPressed(0, 2))
				{
					spawn_bullet(player, Vec2(player->transform->pos.x - 100, player->transform->pos.y));
				}
				else if (sf::Joystick::isButtonPressed(0, 3))
				{
					spawn_bullet(player, Vec2(player->transform->pos.x, player->transform->pos.y - 100));
				}
				else if (sf::Joystick::isButtonPressed(0, 7) || sf::Joystick::isButtonPressed(0, 5))
				{
					if (game_pause_counter % 2 == 0) this->is_game_paused = true;
					else this->is_game_paused = false;
					game_pause_counter++;
					break;
				}
				else if (sf::Joystick::isButtonPressed(0, 6))
				{
					is_game_exit = true;
				}
				else if (sf::Joystick::isButtonPressed(0, 4))
				{
					if (remaining_p_special_power > 0.5)
					{

						player->shape->circle.setFillColor(sf::Color::Red);
						p_special_power_time.restart();
						player->input->special_weapon = true;
						remaining_p_special_power--;

					}

				}


				if (!player->input->special_weapon)
				{
					if (bullet_no > 0)
					{
						bullet_no -= 1;
					}

				}

			}
		}
	}
}
void Game::move_entity()
{
	player->transform->velocity = { 0,0 };
	if (player->input->up)
	{
		player->transform->velocity.y = -350 ;
	}
	if (player->input->down)
	{
		player->transform->velocity.y = 350 ;
	}
	if (player->input->right)
	{
		player->transform->velocity.x = 350 ;
	}
	if (player->input->left)
	{
		player->transform->velocity.x = -350 ;
	}

	player->transform->pos.x += player->transform->velocity.x*delta_time;
	player->transform->pos.y += player->transform->velocity.y * delta_time;
	player->shape->circle.setPosition(player->transform->pos.x, player->transform->pos.y);
	player->shape->circle.rotate(player->transform->angle * delta_time);

	for (auto& e : m_entities.get_entities("enemies"))
	{
		
		e->transform->pos.x += e->transform->velocity.x * delta_time;
		e->transform->pos.y += e->transform->velocity.y * delta_time;
		e->shape->circle.setPosition(e->transform->pos.x, e->transform->pos.y);
		e->shape->circle.rotate(e->transform->angle * delta_time);
	}
	for (auto& b : m_entities.get_entities("bullets"))
	{
		b->transform->pos.x += b->transform->velocity.x * delta_time;
		b->transform->pos.y += b->transform->velocity.y*delta_time;
		b->shape->circle.setPosition(b->transform->pos.x, b->transform->pos.y);
		b->shape->circle.rotate(b->transform->angle);
	}
	for (auto& be : m_entities.get_entities("big_enemies"))
	{
		Vec2 direction;
		direction.x =   player->transform->pos.x - be->transform->pos.x;
		direction.y =  player->transform->pos.y - be->transform->pos.y;
		float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
		if (length > 0)
		{
			direction.x /= length;
			direction.y /= length;
		}
		
		be->transform->pos.x += be->transform->velocity.x*direction.x*delta_time;
		be->transform->pos.y += be->transform->velocity.y * direction.y*delta_time;
		be->shape->circle.setPosition(be->transform->pos.x, be->transform->pos.y);
		be->shape->circle.rotate(be->transform->angle * delta_time);
	}

}

void Game::render(sf::RenderWindow& window)
{
	window.clear();;
	window.draw(background);
	window.draw(player->shape->circle);
	for (auto& e : m_entities.get_entities("enemies"))
	{
		window.draw(e->shape->circle);
	}
	for (auto& be : m_entities.get_entities("big_enemies"))
	{
		window.draw(be->shape->circle);
	}
	for (auto& b : m_entities.get_entities("bullets"))
	{
		window.draw(b->shape->circle);
	}
	if (p_e_collision_count > 2)
	{
		window.draw(game_over_text->write);
	}
	if (player->input->special_weapon)
	{
		window.draw(sp_activated_text->write);
	}
	window.draw(score_text->write);
	window.draw(rem_life_text->write);
	window.draw(bullet_num_text->write);
	window.draw(bullet_reload_time_text->write);
	window.draw(difficulty_level_text->write);
	
	window.display();
}

void Game::spawn_enemy()
{

	//Regular_Enemy
	float e_radius = get_random(10, 20);
	float e_thickness = 5;
	float angle = 200;
	Vec2 e_velocity = { 0,0 };
	Vec2 e_pos = { get_random(50, w_width - 50), get_random(50, w_height - 50) };
	if (score >=0 && score < 80)  e_velocity = { get_random(150,170),get_random(150,170) };
	else if(score >=80&&score<180)  e_velocity = { get_random(170,200),get_random(170,200) };
	else if (score >= 180 && score < 320)  e_velocity = { get_random(200,235),get_random(200,235) };
	else if (score >= 320 && score < 500)  e_velocity = { get_random(235,285),get_random(235,285) };
	else if (score >= 500)  e_velocity = { get_random(285,330),get_random(285,330)};
	auto enemy = m_entities.add_entity("enemies");
	
	enemy->transform = std::make_shared<Ctransform>(e_pos, e_velocity, angle);
	enemy->shape = std::make_shared<Cshape>(e_radius,sf::Color::Black,e_thickness);
	enemy->shape->load_character(get_enemy_image());
	enemy->collision_radius = std::make_shared<Ccollision>(e_radius + e_thickness);


	//Big_Enemy
	if (big_e_spawn_interval % 5==0)
	{
		float be_radius = 23+(big_e_spawn_interval/5);
		float be_thickness = 5;
		float be_angle = 100;
		Vec2 be_pos = { 0,0 };
		int be_spawn_pos =(int) get_random(1, 4);
		if (be_spawn_pos == 1)  be_pos = { get_random(0,w_width),-100 };
		if (be_spawn_pos == 2)  be_pos = { get_random(w_width,w_width+200),get_random(0,w_height)};
		if (be_spawn_pos == 3)  be_pos = { get_random(0,w_width),w_height+100 };
		if (be_spawn_pos == 4)  be_pos = { -100,get_random(0,w_height) };
		Vec2 be_velocity = { float(120+big_e_spawn_interval/1.25), float(120 + big_e_spawn_interval / 1.25) };

		auto big_enemy = m_entities.add_entity("big_enemies");

		big_enemy->transform = std::make_shared<Ctransform>(be_pos,be_velocity, be_angle);
		big_enemy->shape = std::make_shared<Cshape>(be_radius,sf::Color::Black, be_thickness);
		big_enemy->shape->load_character(get_enemy_image());
		big_enemy->collision_radius = std::make_shared<Ccollision>(be_radius+be_thickness);
		big_enemy->life = std::make_shared<Clife>();
		big_enemy->life->set_health(2+(big_e_spawn_interval/5));
		big_e_spawn_sound->set_choices(get_big_e_spawn_sound());
		big_e_spawn_sound->sound.play();
	}
}

void Game::spawn_bullet(std::shared_ptr<Entity>player, const Vec2& target)
{
	

	if (bullet_no > 0)
	{
		float b_radius = 5.0f;
		Vec2 direction;

		auto bullet = m_entities.add_entity("bullets");
		bullet->transform = std::make_shared<Ctransform>(player->transform->pos, Vec2(400, 400), 0.0f);
		bullet->shape = std::make_shared<Cshape>(b_radius, sf::Color::Red, 0.0f);
		bullet->collision_radius = std::make_shared<Ccollision>(b_radius);
		bullet->life = std::make_shared<Clife>(10000, m_current_frame);
		bullet->shape->load_character("bullet");
	
		direction.x = target.x - player->transform->pos.x;
		direction.y = target.y - player->transform->pos.y;
		float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
		if (length > 0)
		{
			direction.x /= length;
			direction.y /= length;
		}
		bullet->transform->velocity *= direction;
		bullet_sound->sound.play();
	}
}

void Game::spawn_player()
{
	float p_radius = 25;
	float p_segment = 30;
	float p_thickness = 5;
	float angle = 0;
	Vec2 p_pos = { w_width / 2, w_height / 2 };
	Vec2 p_velocity = { 0,0 };
	
	auto entity = m_entities.add_entity("player");

	entity->transform = std::make_shared<Ctransform>(p_pos, p_velocity, angle);
	entity->shape = std::make_shared<Cshape>(p_radius,sf::Color::Black, p_thickness);
	entity->input = std::make_shared<Cinput>();
	entity->shape->load_character("player");
	entity->collision_radius = std::make_shared<Ccollision>(p_radius + p_thickness);
	player = entity;
}

void Game::collision()
{
	for (auto& e : m_entities.get_entities("enemies"))
	{
		if (e->transform->pos.x + e->collision_radius->radius > w_width)
		{
			e->transform->velocity.x *= -1;
		}
		if (e->transform->pos.x - e->collision_radius->radius < 0)
		{
			e->transform->velocity.x *= -1;
		}
		if (e->transform->pos.y - e->collision_radius->radius < 0)
		{
			e->transform->velocity.y *= -1;
		}
		if (e->transform->pos.y + e->collision_radius->radius > w_height)
		{
			e->transform->velocity.y *= -1;
		}
	}

	for (auto& p : m_entities.get_entities("player"))
	{
		if (p->transform->pos.x + p->collision_radius->radius > w_width)
		{
			p->transform->pos.x = w_width - p->collision_radius->radius;
		}
		if (p->transform->pos.x - p->collision_radius->radius < 0)
		{
			p->transform->pos.x = p->collision_radius->radius;
		}
		if (p->transform->pos.y + p->collision_radius->radius > w_height)
		{
			p->transform->pos.y = w_height - p->collision_radius->radius;
		}
		if (p->transform->pos.y - p->collision_radius->radius < 0)
		{
			p->transform->pos.y = p->collision_radius->radius;
		}
	}
	
	for (auto& p : m_entities.get_entities("player"))
	{
		for (auto& e : m_entities.get_entities("enemies"))
		{
			if (p->is_active() && e->is_active())
			{
				float dist = p->transform->pos.dist(e->transform->pos);
				if (dist < p->collision_radius->radius + e->collision_radius->radius)
				{
					if (!player->input->special_weapon)
					{
						p_e_collision_count++;
						player_dead_sound->sound.play();
						p->transform->pos = { w_width / 2,w_height / 2 };
						
					}
					score += 5;
					e->destroy();
				}
			}
		}
		for (auto& be : m_entities.get_entities("big_enemies"))
		{
			if (be->is_active() && p->is_active())
			{
				float dist = p->transform->pos.dist(be->transform->pos);
				if (dist < p->collision_radius->radius + be->collision_radius->radius)
				{
					
					if (!player->input->special_weapon)
					{
						p_e_collision_count++;
						player_dead_sound->sound.play();
						p->transform->pos = { w_width / 2,w_height / 2 };
						be->life->health--;
					}
					
					if (be->life->health < 1)
					{
						score += 10;
						be->destroy();
						big_e_dead_sound->sound.play();
					}
					
					
				}

			}
		}

	}
	

	for (auto& b : m_entities.get_entities("bullets"))
	{
		for (auto& e : m_entities.get_entities("enemies"))
		{
			if (e->is_active() && b->is_active())
			{
				float dist = b->transform->pos.dist(e->transform->pos);
				if (dist < e->collision_radius->radius + b->collision_radius->radius)
				{
					score += 5;
					b->destroy();
					e->destroy();
					collision_sound->sound.play();
				}
			}
		}
		for (auto& be : m_entities.get_entities("big_enemies"))
		{
			if (be->is_active() && b->is_active())
			{
				float dist = b->transform->pos.dist(be->transform->pos);
				if (dist < be->collision_radius->radius + b->collision_radius->radius)
				{
					be->life->health--;
					if (be->life->health<1)
					{
						score+=10;
						if (score >= 320 && score < 750 )
						{
							bullet_no += get_random(10, 20);
						}
						else if (score >= 750)
						{
							bullet_no += get_random(20, 30);
						}
						else
						{
							bullet_no += get_random(2, 10);
						}
						
						be->destroy();
						big_e_dead_sound->sound.play();
					}
					b->destroy();
					collision_sound->sound.play();
				}
			}
		}
	}
	
	if (p_e_collision_count > 2)
	{
		is_game_exit = true;
	}

}
void Game::check_bullet_health()
{
	for (auto& b : m_entities.get_entities("bullets"))
	{
		if (b->life->lifespan - (m_current_frame - b->life->frame_created) <= 0)
		{
			b->destroy();
		}
	}
}

void Game::load_high_score()
{
	std::fstream hs_file("high_score.dat", std::ios::in | std::ios::binary);
	if (hs_file)
	{
		hs_file >> high_score;
		hs_file.close();
	}
	else
	{
		std::cout << "cant open";
	}
}

void Game::save_high_score()
{
	if (score > high_score)
	{
		high_score = score;
		std::fstream hs_file("high_score.dat", std::ios::out | std::ios::binary);
		if (hs_file.is_open())
		{
			hs_file << high_score;
			hs_file.close();
		}
	}
}

void Game::change_level_bg_image()
{
	 if (score >= 80 && score < 180) 
	{
		 if (show_bg_image_counter == 1)
		 {
			 
			 if (!background_image.loadFromFile("images/background_image1.jpg"))
			 {
				 std::cout << "background Load failed";
			 }
			 difficulty_level_text->write.setString("Level : Amateur \nNext level at : 180");
			 show_bg_image_counter++;
			 e_spawn_interval = 3.2;
		 }
		 
		
	}
	else if (score >= 180 && score < 320) 
	{
		 if (show_bg_image_counter == 2)
		 {
			
			 if (!background_image.loadFromFile("images/background_image2.jpg"))
			 {
				 std::cout << "background Load failed";
			 }
			 difficulty_level_text->write.setString("Level : Regular \nNext level at : 320");
			 show_bg_image_counter++;
			 e_spawn_interval = 3.0;

		 }
		
	}
	else if (score  >= 320 && score < 500)  
	{
		 if (show_bg_image_counter == 3)
		 {
			 if (!background_image.loadFromFile("images/background_image3.jpg"))
			 {
				 std::cout << "background Load failed";
			 }
			 difficulty_level_text->write.setString("Level : Professional \nNext level at : 500");
			 show_bg_image_counter++;
			 e_spawn_interval = 2.8;

		 }
		
	}
	else if (score >= 500)
	 {
		 if (show_bg_image_counter == 4)
		 {
			 if (!background_image.loadFromFile("images/background_image4.jpg"))
			 {
				 std::cout << "background Load failed";
			 }
			 difficulty_level_text->write.setString("Level : Legend ");
			 e_spawn_interval = 2.5;
			 show_bg_image_counter++;
		 }

	 }
	
}
void Game:: load_special_power()
{
	if (player->input->special_weapon)
	{
		if (p_special_power_time.getElapsedTime().asSeconds()>5)
		{
			player->input->special_weapon = false;
			player->shape->circle.setFillColor(sf::Color::White);
			player->shape->load_character("player");
			
		}
	}
}