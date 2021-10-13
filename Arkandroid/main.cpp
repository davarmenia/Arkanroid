#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
#include "block_macking.h"

#define board_size_w 160.f
#define board_size_h 20.f
#define ball_rad 10.f

#define board_chaps_w 160
#define board_chaps_h 20
#define board_start_pos_x 100
#define board_start_pos_y 550

#define score_area_h 30
#define score_area_pos_x 0
#define score_area_pos_y 0

#define score_text_pos_x 8
#define score_text_pos_y 3

const double pi = 3.141;

int texture_count = 4;
int score_area_w = 100;
int score = 0;
bool ball_move = false;
double ball_move_speed_x = 0;
double ball_move_speed_y = -1;

char ball_move_direction = 'u';
int mouse_move_x = 0;
bool game_start = true;
bool do_animation = false;
int animation_id = 0;

bool _start = true;
bool _win = false;

bool* obj_show = new bool[block_count];
int obj_showed_block_count = 0;

enum TEXTURE_NAMES
{
	ball,
	board,
	score_area,
	block,
	TEXTURE_MAX_COUNT
};

TEXTURE_NAMES texturaner;

void score_area_properties_setting(sf::RectangleShape& score_area);
std::string texture_enum_convert(TEXTURE_NAMES texturaner);

const sf::Texture* obj_texture(TEXTURE_NAMES texturaner) {
	static sf::Texture* texture = new sf::Texture[TEXTURE_NAMES::TEXTURE_MAX_COUNT];
	if (!texture[texturaner].loadFromFile(texture_enum_convert(texturaner)))
		return nullptr;
	const sf::Texture* pTexture = &texture[texturaner];
	texture_count++;
	return pTexture;
}

std::string texture_enum_convert(TEXTURE_NAMES texturaner) {
	switch (texturaner)
	{
	case ball:
		return "ball.png";
		break;
	case board:
		return "board.png";
		break;
	case score_area:
		return "score_area.png";
		break;
	case block:
		return "block.png";
		break;
	default:
		break;
	}
}

void ball_propaerties_setting(sf::CircleShape& ball) {
	ball.setTexture(obj_texture(TEXTURE_NAMES::ball));
}

void board_properties_setting(sf::RectangleShape& board) {
	board.setTexture(obj_texture(TEXTURE_NAMES::board));
	sf::Vector2f board_chaps(board_chaps_w, board_chaps_h);
	board.setSize(board_chaps);
	sf::Vector2f board_position(board_start_pos_x, board_start_pos_y);
	board.setPosition(board_position);
}

void ball_draw(sf::CircleShape& ball, sf::RenderWindow& win) {
	win.draw(ball);
}

void score_text_properties_setting(sf::Text& text) {
	static sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{

	}
	text.setFont(font);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color(0, 0, 0, 185));
	text.setStyle(sf::Text::Bold);
	text.setString("Score: 0");
	sf::Vector2f board_chaps(score_text_pos_x, score_text_pos_y);
	text.setPosition(board_chaps);
}

void score_text_updating(sf::Text& text, sf::RectangleShape& score_area) {
	score++;
	static std::string score_text;
	score_text.clear();
	score_text = "Score: " + std::to_string(score);
	text.setString(score_text);
	score_area_w = text.getLocalBounds().width + 20;
	score_area_properties_setting(score_area);
}

void score_draw(sf::Text& text, sf::RenderWindow& win) {
	win.draw(text);
}

void score_area_properties_setting(sf::RectangleShape& score_area) {
	score_area.setTexture(obj_texture(TEXTURE_NAMES::score_area));
	sf::Vector2f board_chaps(score_area_w, score_area_h);
	score_area.setSize(board_chaps);
	sf::Vector2f board_position(score_area_pos_x, score_area_pos_y);
	score_area.setPosition(board_position);
}

void score_area_drow(sf::RectangleShape& score_area, sf::RenderWindow& win) {
	win.draw(score_area);
}

void game_start_text_properties_setting(sf::Text& text) {
	static sf::Font font;
	if (!font.loadFromFile("Inkfree.ttf"))
	{

	}
	text.setFont(font);
	text.setCharacterSize(68);
	text.setFillColor(sf::Color(0, 0, 0, 185));
	text.setStyle(sf::Text::Bold);
	text.setString("CLICK TO \n    START GAME...");
	sf::Vector2f board_chaps(score_text_pos_x, score_text_pos_y);
	text.setPosition(100, 150);
}

void game_start_text_draw(sf::Text& text, sf::RenderWindow& win) {
	static double _move = 0.01f;
	if (text.getPosition().y >= 200) {
		_move = -_move;
	}
	else if (text.getPosition().y <= 150) {
		_move = -_move;
	}
	text.setPosition(text.getPosition().x, text.getPosition().y - _move);
	win.draw(text);
}

void won_text_properties_setting(sf::Text& text) {
	static sf::Font font;
	if (!font.loadFromFile("Inkfree.ttf"))
	{

	}
	text.setFont(font);
	text.setCharacterSize(68);
	text.setFillColor(sf::Color(0, 0, 0, 185));
	text.setStyle(sf::Text::Bold);
	text.setString("YOU WON");
	sf::Vector2f board_chaps(score_text_pos_x, score_text_pos_y);
	text.setPosition(100, 150);
}

void won_text_draw(sf::Text& text, sf::RenderWindow& win) {
	static double _move = 0.01f;
	if (text.getPosition().y >= 200) {
		_move = -_move;
	}
	else if (text.getPosition().y <= 150) {
		_move = -_move;
	}
	text.setPosition(text.getPosition().x, text.getPosition().y - _move);
	win.draw(text);
}

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Arkandroid Game");

	sf::CircleShape ball(ball_rad);
	sf::RectangleShape board;
	sf::RectangleShape score_area;
	sf::Text text;
	sf::Text text_Game_Start;
	game_start_text_properties_setting(text_Game_Start);
	mackes_blockes();
	ball_propaerties_setting(ball);
	board_properties_setting(board);
	score_text_properties_setting(text);
	score_area_properties_setting(score_area);

	sf::SoundBuffer buffer[2];

	if (!buffer[0].loadFromFile("hit.wav")) {
		std::cout << "ERROR loading andio block_expload";
	}
	if (!buffer[1].loadFromFile("board_hit.wav")) {
		std::cout << "ERROR loading andio board_hit";
	}

	sf::Sound sound;
	

	sf::Texture t;
	if (!t.loadFromFile("wallpaper.jpg"))
		return -1;
	sf::Sprite s(t);

	sf::Texture exploadion;
	exploadion.loadFromFile("animation.png");
	sf::Sprite animation(exploadion);
	sf::Vector2u size = exploadion.getSize();
	animation.setOrigin(0, 0);
	sf::Vector2f increment(0.4f, 0.4f);

	sf::Texture texture_block;
	if (!texture_block.loadFromFile("block.png"))
		return -1;
	const sf::Texture* pTexture_block = &texture_block;

	sf::RectangleShape* block = new sf::RectangleShape[block_count];

	for (int i = 0; i < block_count; i++) {
		block[i].setSize(load_information_for_block_count(i)->size);
		block[i].setFillColor(load_information_for_block_count(i)->color);
		block[i].setPosition(load_information_for_block_count(i)->position);
		block[i].setTexture(obj_texture(TEXTURE_NAMES::block));
		obj_show[i] = true;
		obj_showed_block_count++;
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (_start) {
					_start = false;
				}
				else {
					ball_move = true;
				}
			}
			else if (event.type == sf::Event::MouseMoved) {
				mouse_move_x = sf::Mouse::getPosition(window).x;
			}
		}
		if (ball_move) {
			switch (ball_move_direction)
			{
			case 'u':
				if (ball.getPosition().y <= 0) {
					ball_move_direction = 'd';
					ball_move_speed_y = -ball_move_speed_y;
				}
				break;
			case 'd':
				if (ball.getPosition().y >= board.getGlobalBounds().top - ball.getRadius() * 2 && ball.getPosition().x >= board.getGlobalBounds().left - ball.getRadius() * 2 && ball.getPosition().x <= board.getPosition().x + board.getSize().x) {
					ball_move_direction = 'u';
					ball_move_speed_y = -ball_move_speed_y;
					if (ball.getPosition().x - board.getGlobalBounds().left + ball.getRadius() < 77) {
						int ankyun = ball.getPosition().x - board.getGlobalBounds().left + board.getSize().x / 2 - ball.getRadius() - 50;
						ball_move_speed_x = -(1 - sin(ankyun * pi / 180));
					}
					else if (ball.getPosition().x - board.getGlobalBounds().left + ball.getRadius() > 83) {
						int ankyun = ball.getPosition().x - board.getGlobalBounds().left + board.getSize().x / 2 - ball.getRadius() - 50;
						ball_move_speed_x = (1 - sin(ankyun * pi / 180));
					}
					else {
						ball_move_speed_x = 0;
						ball_move_speed_y = -1;
					}
					sound.setBuffer(buffer[1]);
					sound.play();
				}
				if (ball.getPosition().y + ball.getRadius() * 2 > board.getGlobalBounds().top + 0.1F) {
					std::cout << "Game Over";
					ball_move = false;
				}

				break;
			default:
				break;
			}

			for (int i = 0; i < block_count; ++i) {
				if (ball.getPosition().y <= block[i].getPosition().y + block[i].getSize().y and ball.getPosition().y >= block[i].getPosition().y + block[i].getSize().y - 0.1F and ball.getPosition().x <= block[i].getPosition().x + block[i].getSize().x and ball.getPosition().x + ball.getRadius() * 2 >= block[i].getPosition().x and obj_show[i] == true) { // tak koxm
					ball_move_direction = 'd';
					ball_move_speed_y = -ball_move_speed_y;
					animation.setPosition(block[i].getPosition().x + block[i].getSize().x / 2 - (animation.getTextureRect().width / (animation.getTextureRect().width / animation.getTextureRect().height)) / 2, block[i].getPosition().y - (animation.getTextureRect().height - block[i].getSize().y) / 2);
					do_animation = true;
					score_text_updating(text, score_area);
					//block[i].setPosition(-100, -100);
					obj_show[i] = false;
					obj_showed_block_count--;
					sound.setBuffer(buffer[0]);
					sound.play();
					break;
				}
				else if (ball.getPosition().y + ball.getRadius() * 2 >= block[i].getPosition().y and ball.getPosition().y + ball.getRadius() * 2 <= block[i].getPosition().y + 0.1F and ball.getPosition().x <= block[i].getPosition().x + block[i].getSize().x and ball.getPosition().x + ball.getRadius() * 2 >= block[i].getPosition().x and obj_show[i] == true) { // verev koxm
					ball_move_direction = 'u';
					ball_move_speed_y = -ball_move_speed_y;
					animation.setPosition(block[i].getPosition().x + block[i].getSize().x / 2 - (animation.getTextureRect().width / (animation.getTextureRect().width / animation.getTextureRect().height)) / 2, block[i].getPosition().y - (animation.getTextureRect().height - block[i].getSize().y) / 2);
					do_animation = true;
					score_text_updating(text, score_area);
					//block[i].setPosition(-100, -100);
					obj_show[i] = false;
					obj_showed_block_count--;
					sound.setBuffer(buffer[0]);
					sound.play();
					break;
				}
				else if (ball.getPosition().y <= block[i].getPosition().y + block[i].getSize().y and ball.getPosition().y + ball.getRadius() * 2 >= block[i].getPosition().y and ball.getPosition().x + ball.getRadius() * 2 >= block[i].getPosition().x and ball.getPosition().x <= block[i].getPosition().x + block[i].getSize().x and obj_show[i] == true) { // aj koxm
					ball_move_speed_x = -ball_move_speed_x;
					animation.setPosition(block[i].getPosition().x + block[i].getSize().x / 2 - (animation.getTextureRect().width / (animation.getTextureRect().width / animation.getTextureRect().height)) / 2, block[i].getPosition().y - (animation.getTextureRect().height - block[i].getSize().y) / 2);
					do_animation = true;
					score_text_updating(text, score_area);
					//block[i].setPosition(-100, -100);
					obj_show[i] = false;
					obj_showed_block_count--;
					sound.setBuffer(buffer[0]);
					sound.play();
					break;
				}
			}


			if (ball.getPosition().x >= window.getSize().x - ball.getRadius() * 2) {
				ball_move_speed_x = -ball_move_speed_x;
			}
			else if (ball.getPosition().x <= 0) {
				ball_move_speed_x = -ball_move_speed_x;
			}

			ball.setPosition(ball.getPosition().x + ball_move_speed_x / 4, ball.getPosition().y + ball_move_speed_y / 8);
			game_start = false;
		}

		if (game_start) {
			ball.setPosition(board.getGlobalBounds().left + board.getSize().x / 2 - ball.getRadius(), board.getGlobalBounds().top - ball.getRadius() * 2);
		}

		if (mouse_move_x < board.getSize().x / 2) {
			board.setPosition(0, board.getPosition().y);
		}
		else if (mouse_move_x > window.getSize().x - board.getSize().x / 2) {
			board.setPosition(window.getSize().x - board.getSize().x, board.getPosition().y);
		}
		else {
			board.setPosition(mouse_move_x - board.getSize().x / 2, board.getPosition().y);
		}

		if (!obj_showed_block_count) {
			won_text_properties_setting(text_Game_Start);
			_win = true;
		}

		window.clear(sf::Color(65, 130, 120, 155));
		window.draw(s);

		if (_start or _win) {
			if (_start) {
				game_start_text_draw(text_Game_Start, window);
			}
			else {
				won_text_draw(text_Game_Start, window);
			}
		}
		else {
			score_area_drow(score_area, window);
			score_draw(text, window);
			ball_draw(ball, window);

			for (int i = 0; i < block_count; i++) {
				if (obj_show[i]) {
					window.draw(block[i]);
				}
			}

			if (do_animation) {
				if (animation_id < 2112) {
					animation.setTextureRect(sf::IntRect(static_cast<int>(animation_id) / 192 * 101, 0, 101, 101));
					window.draw(animation);
					animation_id += 1;
				}
				else {
					do_animation = false;
					animation_id = 0;
				}
			}

			window.draw(board);
		}

		window.display();
	}


	return 0;
}