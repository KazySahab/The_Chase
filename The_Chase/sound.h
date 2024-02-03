#pragma once
#include "iostream"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

class Sound
{
public:
	sf::Music sound;
	bool is_paused = false;
	Sound(){}

	Sound(bool to_repeat,const float vol){
		sound.setLoop(to_repeat);
		sound.setVolume(vol);
	}
	Sound(const std::string name, bool to_repeat,const float vol )
	{
		if (!sound.openFromFile("music/" + name + ".mp3"))
		{
			std::cout << "cant open audio file" << name << std::endl;
		}
		else
		{
			sound.setLoop(to_repeat);
			sound.setVolume(vol);
		}
		
	}
	void set_choices(const std::string name)
	{
		if (!sound.openFromFile("music/" + name + ".mp3"))
		{
			std::cout << "cant open audio file" << name << std::endl;
		}
	}
};

