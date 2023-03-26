#pragma once
#include <SFML/Graphics.hpp>
#include "GLOBALS.h"

using namespace sf;

class Input
{

public:

	bool kd_UP = false;
	bool kd_LEFT = false;
	bool kd_DOWN = false;
	bool kd_RIGHT = false;

	Input(int controlSchemeId = 0) {
		switch (controlSchemeId) {
			case(0):
			{
				//Arrow Keys
				keyMap_up = sf::Keyboard::W;
				keyMap_left = sf::Keyboard::A;
				keyMap_down = sf::Keyboard::S;
				keyMap_right = sf::Keyboard::D;
				break;
			}
			case(1):
			{
				//WASD

				keyMap_up = sf::Keyboard::Up;
				keyMap_left = sf::Keyboard::Left;
				keyMap_down = sf::Keyboard::Down;
				keyMap_right = sf::Keyboard::Right;
				break;
			}
			case(2):
			{
				//IJKL
				keyMap_up = sf::Keyboard::I;
				keyMap_left = sf::Keyboard::J;
				keyMap_down = sf::Keyboard::K;
				keyMap_right = sf::Keyboard::L;
				break;
			}
			case(3):
			{
				//Numpad 8456 (NUM LOCK MUST BE ON)
				keyMap_up = sf::Keyboard::Numpad8;
				keyMap_left = sf::Keyboard::Numpad4;
				keyMap_down = sf::Keyboard::Numpad5;
				keyMap_right = sf::Keyboard::Numpad6;
				break;
			}
			default:
			{
				//Arrow Keys
				keyMap_up = sf::Keyboard::Up;
				keyMap_left = sf::Keyboard::Left;
				keyMap_down = sf::Keyboard::Down;
				keyMap_right = sf::Keyboard::Right;
				break;
			}
		}
	}

	

	virtual void Update();
	


private:
	bool lf_upPressed = false;
	bool lf_leftPressed = false;
	bool lf_downPressed = false;
	bool lf_rightPressed = false;

	sf::Keyboard::Key keyMap_up;
	sf::Keyboard::Key keyMap_left;
	sf::Keyboard::Key keyMap_down;
	sf::Keyboard::Key keyMap_right;
};

