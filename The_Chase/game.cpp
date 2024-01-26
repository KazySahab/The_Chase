#include "game.h"


const float w_width = sf::VideoMode::getDesktopMode().width;
const float w_height = sf::VideoMode::getDesktopMode().height;

Game::Game()
	:window(sf::VideoMode(w_width, w_height), "Don't Touch")

{
	if (!font.loadFromFile("font/roboto.ttf"))
	{
		throw std::runtime_error("Failed to load font.");
	}

	score_text.setFont(font);
	score_text.setCharacterSize(24);
	score_text.setFillColor(sf::Color::White);
	score_text.setPosition(sf::Vector2f{ 50,10 });

	bullet_num.setFont(font);
	bullet_num.setCharacterSize(24);
	bullet_num.setFillColor(sf::Color::White);
	bullet_num.setPosition(sf::Vector2f{ w_width - 300,10 });

	bullet_reload_time.setFont(font);
	bullet_reload_time.setCharacterSize(24);
	bullet_reload_time.setFillColor(sf::Color::White);
	bullet_reload_time.setPosition(sf::Vector2f{ w_width - 300,50 });

	help_text.setFont(font);
	help_text.setCharacterSize(24);
	help_text.setFillColor(sf::Color::White);
	help_text.setPosition(sf::Vector2f{ w_width / 2 - 120,10 });
}


void Game::run()
{
	spawn_player();

	while (!is_game_exit)
	{
		sf::Time frame_time = delta_clock.restart();
		delta_time = frame_time.asSeconds();
		try
		{
			handle_input();
			m_entities.update();
			if (e_spawn_time.getElapsedTime().asSeconds() > e_spawn_interval)
			{
				spawn_enemy();
				e_spawn_time.restart();
			}
			if (bullet_time.getElapsedTime().asSeconds() > 30)
			{
				bullet_no += 30;
				bullet_time.restart();
			}
			help_text.setString("Press 'ESC' to exit or Hold 'P' to pause");
			score_text.setString("Score : " + std::to_string(score));
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


		catch (const std::exception& e)
		{
			std::cerr << "Unhandled exception: " << e.what() << std::endl;
		}
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
			{
				player->input->up = true;
				break;

			}
			case sf::Keyboard::A:
			{
				player->input->left = true;
				break;

			}
			case sf::Keyboard::S:
			{
				player->input->down = true;
				break;

			}
			case sf::Keyboard::D:
			{
				player->input->right = true;
				break;

			}
			case sf::Keyboard::P:
			{
				this->is_game_paused = true;
				break;
			}
			case sf::Keyboard::Escape:
			{
				this->is_game_exit = true;
				break;
			}
			}
		}
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
			{
				player->input->up = false;
				break;

			}
			case sf::Keyboard::A:
			{
				player->input->left = false;
				break;

			}
			case sf::Keyboard::S:
			{
				player->input->down = false;
				break;

			}
			case sf::Keyboard::D:
			{
				player->input->right = false;
				break;

			}
			case sf::Keyboard::P:
			{
				this->is_game_paused = false;
				break;
			}
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

}

void Game::render()
{
	window.clear();
	window.draw(player->shape->circle);
	for (auto& e : m_entities.get_entities("enemies"))
	{
		window.draw(e->shape->circle);
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
	float e_radius = get_random(10, 20);
	float e_segment = get_random(3, 7);
	float e_thickness = 3;
	Vec2 e_pos = { get_random(50, w_width - 50), get_random(50, w_height - 50) };
	Vec2 e_velocity = { get_random(200,250),get_random(200,250) };
	float angle = 200;

	sf::Color e_fill_color = { sf::Uint8(get_random(0, 255)),
						  sf::Uint8(get_random(0, 255)),
						  sf::Uint8(get_random(0, 255)) };

	auto enemy = m_entities.add_entity("enemies");
	enemy->transform = std::make_shared<Ctransform>(e_pos, e_velocity, angle);
	enemy->shape = std::make_shared<Cshape>(e_radius, e_segment, e_fill_color, sf::Color::Red, e_thickness);
	enemy->collision_radius = std::make_shared<Ccollision>(e_radius + e_thickness);
	enemy->transform->velocity *= delta_time;
	score += 5;
}

void Game::spawn_bullet(std::shared_ptr<Entity>player, const Vec2& target)
{
	if (bullet_no > 0)
	{
		float b_radius = 5.0f;
		auto bullet = m_entities.add_entity("bullets");
		bullet->transform = std::make_shared<Ctransform>(player->transform->pos, Vec2(300, 300), 0.0f);
		bullet->shape = std::make_shared<Cshape>(b_radius, 30.0f, sf::Color::White, sf::Color::Red, 0.0f);
		bullet->collision_radius = std::make_shared<Ccollision>(b_radius);
		bullet->life = std::make_shared<Clife>(10000, m_current_frame);

		Vec2 direction;
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
	float p_radius = 20;
	float p_segment = 7;
	float p_thickness = 4;
	Vec2 p_pos = { w_width / 2, w_height / 2 };
	Vec2 p_velocity = { 0,0 };
	float angle = 200;
	auto entity = m_entities.add_entity("player");
	entity->transform = std::make_shared<Ctransform>(p_pos, p_velocity, angle);
	entity->shape = std::make_shared<Cshape>(p_radius, p_segment, sf::Color::Black, sf::Color::Red, p_thickness);
	entity->input = std::make_shared<Cinput>();
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
			float dist = p->transform->pos.dist(e->transform->pos);
			if (dist < p->collision_radius->radius + e->collision_radius->radius)
			{
				p->transform->pos = { w_width / 2,w_height / 2 };
				if (score > 20)
				{
					score -= 20;
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

float get_random(float min, float max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}
