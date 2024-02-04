#include "main_menu.h"
Main_menu::Main_menu()
	:menu_window(sf::VideoMode(w_width, w_height), "Main Menu")
{
	if (!font.loadFromFile("font/h_ghost.ttf"))
	{
		std::cout << "Failed to load Font" << std::endl;
	}
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
	main_menu_selection[3].setString("EXIT");
}

void Main_menu::draw_menu()
{
	for (int i = 0; i < n_options; i++)
	{
		menu_window.draw(main_menu_selection[i]);
	}
}

void Main_menu::run()
{
	main_menu_selection[0].setFillColor(sf::Color::Red);
	menu_bg_sound->sound.play();
	while (menu_window.isOpen())
	{

		while (menu_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				menu_window.close();
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
					break;
				}

				case sf::Keyboard::Up:
				{
					main_menu_selection[selected].setFillColor(sf::Color::White);
					select_counter--;
					if (select_counter < 0) select_counter = 3;
					selected = select_counter % n_options;
					main_menu_selection[selected].setFillColor(sf::Color::Red);
					break;
				}
				case sf::Keyboard::Enter:
				{
					switch (selected)
					{
					case 0:
						
						menu_bg_sound->sound.stop();
						is_play_triggerred = true;
						menu_window.close();
						
						break;
					case 1:
						std::cout << "Options Triggered";
						break;

					case 2:
						load_menu_high_score();
						menu_h_score_text->write.setString("HIGH SCORE : " + std::to_string(menu_high_score));
						menu_h_score_text->write.setCharacterSize(35);
						std::cout << "yes written" << std::endl;
						menu_window.draw(menu_h_score_text->write);
						break;
					case 3:
						menu_window.close();
						break;
					}
				}

				}
			}
		}
		menu_window.clear(sf::Color::Black);
		draw_menu();
		menu_window.display();
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
	else
	{
		std::cout << "cant open";
	}
}