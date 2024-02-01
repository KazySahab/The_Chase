#include "game.h"

const float w_width = sf::VideoMode::getDesktopMode().width;
const float w_height = sf::VideoMode::getDesktopMode().height;

Game::Game()
	:window(sf::VideoMode(w_width, w_height), "The Chase")
{
	if (!font.loadFromFile("font/roboto.ttf"))
	{
		throw std::runtime_error("Failed to load font.");
	}
	if (!background_image.loadFromFile("images/background_image.jpg"))
	{
		std::cout << "background Load failed";
	}

	score_text.setFont(font);
	score_text.setCharacterSize(24);
	score_text.setStyle(sf::Text::Bold);
	score_text.setFillColor(sf::Color::Black);
	score_text.setPosition(sf::Vector2f{ 50,10 });

	bullet_num.setFont(font);
	bullet_num.setCharacterSize(24);
	bullet_num.setStyle(sf::Text::Bold);
	bullet_num.setFillColor(sf::Color::Black);
	bullet_num.setPosition(sf::Vector2f{ w_width - 300,10 });

	bullet_reload_time.setFont(font);
	bullet_reload_time.setCharacterSize(24);
	bullet_reload_time.setStyle(sf::Text::Bold);
	bullet_reload_time.setFillColor(sf::Color::Black);
	bullet_reload_time.setPosition(sf::Vector2f{ w_width - 300,50 });

	help_text.setFont(font);
	help_text.setCharacterSize(24);
	help_text.setStyle(sf::Text::Bold);
	help_text.setFillColor(sf::Color::Black);
	help_text.setPosition(sf::Vector2f{ w_width / 2 - 120,10 });

	background.setSize(sf::Vector2f(w_width, w_height));
	background.setTexture(&background_image);
}


void Game::run()
{
	spawn_player();

	while (!is_game_exit)
	{
		sf::Time frame_time = delta_clock.restart();
		delta_time = frame_time.asSeconds();

		
			handle_input();
			m_entities.update();
			if (e_spawn_time.getElapsedTime().asSeconds() > e_spawn_interval)
			{
				big_e_spawn_interval++;
				spawn_enemy();
				e_spawn_time.restart();
			}
			if (bullet_time.getElapsedTime().asSeconds() > 30)
			{
				bullet_no += 30;
				bullet_time.restart();
			}
			help_text.setString("Press 'ESC' to exit or Hold 'P' to pause");
			score_text.setString("Score : " + std::to_string(score) + "\n Life :" + std::to_string(3 - p_e_collision_count));
			bullet_reload_time.setString("Reload Time (30s) : " + std::to_string((int)bullet_time.getElapsedTime().asSeconds()));
			bullet_num.setString("Remaining Bullets : " + std::to_string(bullet_no));
			
			if (!is_game_paused)
			{
				move_entity();
				collision();
				render();
			}
			life_spawn();
			m_current_frame++;

		
	}
}

void Game::handle_input()
{

	while (window.pollEvent(event)) {


		if (event.type == sf::Event::Closed)
			window.close();

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
					this->is_game_paused = true;
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
				
				case sf::Keyboard::P:
					this->is_game_paused = false;
					break;
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawn_bullet(player, Vec2(event.mouseButton.x, event.mouseButton.y));
				if (bullet_no > 0)
				{
					bullet_no -= 1;
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

				if (bullet_no > 0)
				{
					bullet_no -= 1;
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
		player->transform->velocity.y = -350 * delta_time;
	}
	if (player->input->down)
	{
		player->transform->velocity.y = 350 * delta_time;
	}
	if (player->input->right)
	{
		player->transform->velocity.x = 350 * delta_time;
	}
	if (player->input->left)
	{
		player->transform->velocity.x = -350 * delta_time;
	}

	player->transform->pos += player->transform->velocity;
	player->shape->circle.setPosition(player->transform->pos.x, player->transform->pos.y);
	player->shape->circle.rotate(player->transform->angle * delta_time);

	for (auto& e : m_entities.get_entities("enemies"))
	{
		e->transform->pos += e->transform->velocity;
		e->shape->circle.setPosition(e->transform->pos.x, e->transform->pos.y);
		e->shape->circle.rotate(e->transform->angle * delta_time);
	}
	for (auto& b : m_entities.get_entities("bullets"))
	{
		b->transform->pos += b->transform->velocity;
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

void Game::render()
{
	window.clear();
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
	window.draw(score_text);
	window.draw(bullet_num);
	window.draw(bullet_reload_time);
	window.draw(help_text);
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
	if (score >=0 && score< 100)  e_velocity = { get_random(150,200),get_random(150,200) };
	else if(score>100&&score<300)  e_velocity = { get_random(180,220),get_random(180,220) };
	else if (score > 300 && score < 500)  e_velocity = { get_random(200,250),get_random(200,250) };
	else if (score > 500)  e_velocity = { get_random(250,300),get_random(250,300) };
	auto enemy = m_entities.add_entity("enemies");
	
	enemy->transform = std::make_shared<Ctransform>(e_pos, e_velocity, angle);
	enemy->shape = std::make_shared<Cshape>(e_radius,sf::Color::Black,e_thickness);
	enemy->shape->load_character(get_enemy_image());
	enemy->collision_radius = std::make_shared<Ccollision>(e_radius + e_thickness);
	enemy->transform->velocity *= delta_time;

	//Big_Enemy
	if (big_e_spawn_interval % 5==0)
	{
		float be_radius = 25+(big_e_spawn_interval/5);
		float be_thickness = 5;
		float be_angle = 100;
		Vec2 be_pos = { w_width / 2,w_height / 2 };
		Vec2 be_velocity = { float(120+big_e_spawn_interval/1.25), float(120 + big_e_spawn_interval / 1.25) };

		auto big_enemy = m_entities.add_entity("big_enemies");

		big_enemy->transform = std::make_shared<Ctransform>(be_pos,be_velocity, be_angle);
		big_enemy->shape = std::make_shared<Cshape>(be_radius,sf::Color::Red, be_thickness);
		big_enemy->shape->load_character(get_enemy_image());
		big_enemy->collision_radius = std::make_shared<Ccollision>(be_radius+be_thickness);
		big_enemy->life = std::make_shared<Clife>();
		big_enemy->life->set_health(3+(big_e_spawn_interval/5));
	}
}

void Game::spawn_bullet(std::shared_ptr<Entity>player, const Vec2& target)
{
	if (bullet_no > 0)
	{
		float b_radius = 5.0f;
		Vec2 direction;

		auto bullet = m_entities.add_entity("bullets");
		bullet->transform = std::make_shared<Ctransform>(player->transform->pos, Vec2(300, 300), 0.0f);
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
		bullet->transform->velocity *= delta_time;
	}
}

void Game::spawn_player()
{
	float p_radius = 25;
	float p_segment = 30;
	float p_thickness = 2;
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
					p_e_collision_count++;
					p->transform->pos = { p->collision_radius->radius,p->collision_radius->radius };
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
					be->life->health--;
					p_e_collision_count++;
					if (be->life->health < 1)
					{
						score += 10;
						be->destroy();
					}
					
					p->transform->pos = { p->collision_radius->radius,p->collision_radius->radius };

				}

			}
		}
		if (p_e_collision_count > 2)
		{
			sf::sleep(sf::seconds(3));
			window.close();
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
						be->destroy();
					}
					b->destroy();
				}
			}
		}
	}
}
void Game::life_spawn()
{
	for (auto& b : m_entities.get_entities("bullets"))
	{
		if (b->life->lifespan - (m_current_frame - b->life->frame_created) < 0)
		{
			b->destroy();
		}
	}
}


void Game:: additional_game_functions()
{
	

}

std::string get_enemy_image()
{
	int e_choose = (int)get_random(1, 6);
	switch (e_choose)
	{
		case 1:
			return "enemy1";
			break;
		case 2:
			return "enemy2";
			break;
		case 3:
			return "enemy3";
			break;
		case 4:
			return "enemy4";
			break;
		case 5:
			return "enemy5";
			break;
	}
}

float get_random(float min, float max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}