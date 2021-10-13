#pragma once
#include <fstream>
#include <string>

int block_count = 0;
sf::Vector2f block_chaps(50.f, 20.f);

enum BLOCK_TYPE
{
	standart,
	board_wight_up,
	board_wight_down,
	fire_ball,
	god_mod
};

struct BLOCK_PROPERTIES
{
	sf::Vector2f size;
	int id;
	sf::Color color;
	sf::Vector2f position;
	BLOCK_TYPE type;
};

BLOCK_TYPE enum_conv(int block_id) {
	switch (block_id)
	{
	case standart:
		return BLOCK_TYPE::standart;
		break;
	case board_wight_up:
		return BLOCK_TYPE::board_wight_up;
		break;
	case board_wight_down:
		return BLOCK_TYPE::board_wight_down;
		break;
	case fire_ball:
		return BLOCK_TYPE::fire_ball;
		break;
	case god_mod:
		return BLOCK_TYPE::god_mod;
		break;
	default:
		break;
	}
}

void load_information_for_blocks(BLOCK_PROPERTIES* obj_B_pr) {
	std::ifstream file("level_1.txt");
	std::string str;
	while (std::getline(file, str))
	{
		size_t pos = 0;
		std::string token;
		int sbustr_id = 0;
		std::string _token;
		int bl_col_arr[3];
		int col_id = 0;
		int _pos = 0;

		int bl_pos_arr[2];
		int pos_id = 0;

		sf::Vector2f pos_v(bl_pos_arr[0], bl_pos_arr[1]);

		obj_B_pr->size = block_chaps;
		while ((pos = str.find("/")) != std::string::npos) {
			token = str.substr(0, pos);
			//std::cout << token << std::endl;
			if (sbustr_id == 0) {
				obj_B_pr->id = std::stoi(str);
			}
			str.erase(0, pos + 1);
			switch (sbustr_id)
			{
			case 0:

				break;
			case 1:

				while ((_pos = token.find(",")) != std::string::npos) {
					_token = token.substr(0, _pos);
					//std::cout << _token << std::endl;
					token.erase(0, _pos + 1);
					bl_col_arr[col_id] = std::stoi(_token);
					col_id++;
				}
				bl_col_arr[col_id] = std::stoi(token);
				obj_B_pr->color = sf::Color(bl_col_arr[0], bl_col_arr[1], bl_col_arr[2]);
				break;
			case 2:
				_pos = 0;
				while ((_pos = token.find(",")) != std::string::npos) {
					_token = token.substr(0, _pos);
					//std::cout << _token << std::endl;
					token.erase(0, _pos + 1);
					bl_pos_arr[pos_id] = std::stoi(_token);
					pos_id++;
				}
				bl_pos_arr[pos_id] = std::stoi(token);
				obj_B_pr->position = sf::Vector2f(bl_pos_arr[0], bl_pos_arr[1]);
				break;
			default:
				break;
			}
			sbustr_id++;
		}
		obj_B_pr->type = enum_conv(std::stoi(str));
		++obj_B_pr;
	}
}

const BLOCK_PROPERTIES* make(int id) {
	if (id == -1) {
		std::ifstream file("level_1.txt");
		std::string str;
		while (std::getline(file, str))
		{
			block_count++;
		}
	}
	//std::cout << block_count << std::endl;
	static BLOCK_PROPERTIES* obj_BLOCK_PROPERTIES = new BLOCK_PROPERTIES[block_count];
	if (id == -1) {
			load_information_for_blocks(obj_BLOCK_PROPERTIES);
	}
	const BLOCK_PROPERTIES* pobj_BLOCK_PROPERTIES = &obj_BLOCK_PROPERTIES[id];

	return pobj_BLOCK_PROPERTIES;
}

const BLOCK_PROPERTIES* load_information_for_block_count(int id) {
	return make(id);
}

void mackes_blockes() {
	make(-1);
}