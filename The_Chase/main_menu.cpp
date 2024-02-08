#include "main_menu.h"
Main_menu::Main_menu()
{
	if (!icon.loadFromFile("images/icon.png"))
	{
		std::cout << "no icon" << std::endl;
	}
	if (!font.loadFromFile("font/h_ghost.ttf"))
	{
		std::cout << "Failed to load Font" << std::endl;
	}
	if (!menu_bg_image.loadFromFile("images/background_image.jpg"))
	{
		std::cout << "background Load failed";
	}
	
	
	menu_background.setSize(sf::Vector2f(w_width, w_height));
	menu_background.setTexture(&menu_bg_image);

	for (int i = 0; i < n_options; i++)
	{
		main_menu_selection[i].setFont(font);
		main_menu_selection[i].setCharacterSize(30);
		main_menu_selection[i].setStyle(sf::Text::Bold);
		main_menu_selection[i].setFillColor(sf::Color::White);
		main_menu_selection[i].setPosition(w_width / 2.2, w_height / 3 + i * 100);
	}
	main_menu_selection[0].setString("PLAY");
	main_menu_selection[1].setString("INSTRUCTIONS");
	main_menu_selection[2].setString("HIGH SCORE");
	main_menu_selection[3].setString("ABOUT US");
	main_menu_selection[4].setString("EXIT");

	window.create(sf::VideoMode(w_width, w_height), "Main Menu");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Main_menu::draw_menu()
{
	for (int i = 0; i < n_options; i++)
	{
		window.draw(main_menu_selection[i]);
	}
}

void Main_menu::run()
{

	main_menu_selection[0].setFillColor(sf::Color::Red);
	scary_welcome->sound.play();
	menu_bg_sound->sound.play();
	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == event.KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Down:
				{

					main_menu_selection[selected].setFillColor(sf::Color::White);
					select_counter++;
					selected = select_counter % n_options;
					main_menu_selection[selected].setFillColor(sf::Color::Red);
					menu_scroll->sound.play();
					break;
				}

				case sf::Keyboard::Up:
				{
					main_menu_selection[selected].setFillColor(sf::Color::White);
					select_counter--;
					if (select_counter < 0) select_counter = n_options-1;
					selected = select_counter % n_options;
					main_menu_selection[selected].setFillColor(sf::Color::Red);
					menu_scroll->sound.play();
					break;
				}
				case sf::Keyboard::Enter:
				{
					switch (selected)
					{
					case 0:

						menu_bg_sound->sound.setVolume(0);
						scary_welcome->sound.stop();
						menu_select->sound.play();
						is_play_triggerred = true;
						break;
					case 1:
						menu_select->sound.play();
						is_instruction_triggerrred = true;
						break;
					case 2:
						menu_select->sound.play();
						is_high_score_triggerred = true;
						break;
					case 3:
						menu_select->sound.play();
						is_about_us_triggerred = true;
						break;
					case 4:
						menu_select->sound.play();
						window.close();
						break;
					}
				}

				}

			}

			if (sf::Joystick::isConnected(0))
			{
				if (event.type == sf::Event::JoystickMoved)
				{
					float j_move_y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
					if (j_move_y == 100)
					{
						main_menu_selection[selected].setFillColor(sf::Color::White);
						select_counter++;
						selected = select_counter % n_options;
						main_menu_selection[selected].setFillColor(sf::Color::Red);
						menu_scroll->sound.play();
					}
					else if (j_move_y == -100)
					{
						main_menu_selection[selected].setFillColor(sf::Color::White);
						select_counter--;
						if (select_counter < 0) select_counter = n_options-1;
						selected = select_counter % n_options;
						main_menu_selection[selected].setFillColor(sf::Color::Red);
						menu_scroll->sound.play();
					}
				}
				if (event.type == sf::Event::JoystickButtonPressed)
				{
					if (sf::Joystick::isButtonPressed(0, 0))
					{
						switch (selected)
						{
						case 0:
							menu_bg_sound->sound.setVolume(0);
							menu_select->sound.play();
							is_play_triggerred = true;
							break;
						case 1:
							menu_select->sound.play();
							is_instruction_triggerrred = true;
							break;
						case 2:
							menu_select->sound.play();
							is_high_score_triggerred = true;
							break;
						case 3:
							menu_select->sound.play();
							is_about_us_triggerred = true;
							break;
						case 4:
							menu_select->sound.play();
							window.close();
							break;
						}
					}
				}
			}
		}
		load_scene();
		window.clear(sf::Color::Black);
		window.draw(menu_background);
		draw_menu();
		
		window.display();
	}

}

void Main_menu::load_menu_high_score()
{
	
	std::fstream hs_file("high_score.dat", std::ios::in | std::ios::binary);
	if (hs_file)
	{
		hs_file >> menu_high_score;
		hs_file.close();
	}
}

void Main_menu::load_scene()
{
	if (is_play_triggerred)
	{
		Game game;
		game.run(window);
		menu_bg_sound->sound.setVolume(30);
		is_play_triggerred = false;
	}

	if (is_high_score_triggerred)
	{
		if (!menu_bg_image.loadFromFile("images/background_image.jpg"))
		{
			std::cout << "background Load failed";
		}

		menu_background.setSize(sf::Vector2f(w_width, w_height));
		menu_background.setTexture(&menu_bg_image);
		window.setTitle("High Score");
		while (window.isOpen())
		{
			sf::Event hs_event;
			while (window.pollEvent(hs_event))
			{
				if (hs_event.type == sf::Event::Closed)
					window.close();
			}

			if (sf::Joystick::isButtonPressed(0, 6) || sf::Joystick::isButtonPressed(0, 4))
			{
				is_high_score_triggerred = false;
				window.setTitle("Main Menu");
				menu_select->sound.play();
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				is_high_score_triggerred = false;
				window.setTitle("Main Menu");
				menu_select->sound.play();
				break;

			}
			load_menu_high_score();
			window.clear(sf::Color::Black);
			window.draw(menu_background);
			menu_h_score_text->write.setString("HIGH SCORE : " + std::to_string(menu_high_score));
			menu_h_score_text->write.setCharacterSize(35);
			window.draw(menu_h_score_text->write);
			window.display();
		}
	}

	if (is_instruction_triggerrred)
	{
		std::shared_ptr<Show_text>instruction_keyboard = std::make_shared<Show_text>(sf::Vector2f(w_width/2-200, 100));
		std::shared_ptr<Show_text>instruction_joystick = std::make_shared<Show_text>(sf::Vector2f(w_width /2-200, w_height/2+100));
		if (!menu_bg_image.loadFromFile("images/background_image.jpg"))
		{
			std::cout << "background Load failed";
		}
		menu_background.setSize(sf::Vector2f(w_width, w_height));
		menu_background.setTexture(&menu_bg_image);
		window.setTitle("Instructions");
		instruction_keyboard->write.setString("\t----KEYBOARD---- \n\n W - Up \n A - Left \n S - Down \n D - Right \n P - Pause/Play \n Esc - Back/Exit \n Left Click - Fire/Shoot \n Right Click - Special Power");
		instruction_keyboard->write.setLetterSpacing(2);
		instruction_keyboard->write.setCharacterSize(30);
		instruction_joystick->write.setString("\t ----JOYSTICK---- \n\n Left Analog - Move Player \n Right Buttons (X,Y,A,B) - Fire/Shoot \n Start/RB - Pause/Play \n Back/LB - Back/Exit \n LB - Special Power");
		instruction_joystick->write.setLetterSpacing(2);
		instruction_joystick->write.setCharacterSize(30);
		while (window.isOpen())
		{
			sf::Event inst_event;
			while (window.pollEvent(inst_event))
			{
				if (inst_event.type == sf::Event::Closed)
					window.close();
			}

			if (sf::Joystick::isButtonPressed(0, 6) || sf::Joystick::isButtonPressed(0, 4))
			{
				is_instruction_triggerrred = false;
				window.setTitle("Main Menu");
				menu_select->sound.play();
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				is_instruction_triggerrred = false;
				window.setTitle("Main Menu");
				menu_select->sound.play();
				break;

			}
			window.clear(sf::Color::Black);
			window.draw(menu_background);
			window.draw(instruction_keyboard->write);
			window.draw(instruction_joystick->write);
			window.display();
		}

	}

	if(is_about_us_triggerred)
	{
		if (!menu_bg_image.loadFromFile("images/background_image.jpg"))
		{
			std::cout << "background Load failed";
		}
		std::shared_ptr<Show_text> about_us = std::make_shared<Show_text>(sf::Vector2f(w_width/3.5,w_height/3.5));
		menu_background.setSize(sf::Vector2f(w_width, w_height));
		menu_background.setTexture(&menu_bg_image);
		window.setTitle("About Us");
		while (window.isOpen())
		{
			sf::Event hs_event;
			while (window.pollEvent(hs_event))
			{
				if (hs_event.type == sf::Event::Closed)
					window.close();
			}

			if (sf::Joystick::isButtonPressed(0, 6) || sf::Joystick::isButtonPressed(0, 4))
			{
				is_about_us_triggerred = false;
				window.setTitle("Main Menu");
				menu_select->sound.play();
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				is_about_us_triggerred = false;
				window.setTitle("Main Menu");
				menu_select->sound.play();
				break;

			}
			about_us->write.setString("\t\t\t\t\tABOUT US \n\n Hello There! and welcome to the game.... \n This is a game developed by a students of 2nd semester by using c++. \n Feel free to play the game. \n Any kind of response and feedback is acceptable. \n\n Developer - MAUSHAM NEUPANE \n Logic Manager - MOHAMMAD AARMAN \n Graphic Designer - ISHAN KANAUJIYA ");
			window.clear(sf::Color::Black);
			window.draw(menu_background);
			about_us->write.setCharacterSize(35);
			window.draw(about_us->write);
			window.display();
		}
	}
}