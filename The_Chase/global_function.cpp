#include "global_function.h"

float get_random(float min, float max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

std::string get_big_e_spawn_sound()
{
	int be_choose = (int)get_random(1, 4);
	std::cout << be_choose << std::endl;
	switch (be_choose)
	{
	case 1:
		return "big_e_spawn1";
		break;
	case 2:
		return "big_e_spawn2";
		break;
	case 3:
		return "big_e_spawn3";
		break;
	}
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