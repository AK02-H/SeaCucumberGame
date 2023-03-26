#include "Input.h"

void Input::Update() {
	if (sf::Keyboard::isKeyPressed(keyMap_left)) {
			
			if (!lf_leftPressed) {
				kd_LEFT = true;
				lf_leftPressed = true;
			}
			else {
				kd_LEFT = false;
			}

	}
	else if (sf::Keyboard::isKeyPressed(keyMap_right)) {
		if (!lf_rightPressed) {
			kd_RIGHT = true;
			lf_rightPressed = true;
		}
		else {
			kd_RIGHT = false;
		}
	}
	else if (sf::Keyboard::isKeyPressed(keyMap_up)) {
		if (!lf_upPressed) {
			kd_UP = true;
			lf_upPressed = true;
		}
		else {
			kd_UP = false;
		}
	}
	else if (sf::Keyboard::isKeyPressed(keyMap_down)) {
		if (!lf_downPressed) {
			kd_DOWN = true;
			lf_downPressed = true;
		}
		else {
			kd_DOWN = false;
		}
	}
		
	if (!sf::Keyboard::isKeyPressed(keyMap_left)) {
		kd_LEFT = false;
		lf_leftPressed = false;
	}
	if (!sf::Keyboard::isKeyPressed(keyMap_right)) {
		kd_RIGHT = false;
		lf_rightPressed = false;
	}
	if (!sf::Keyboard::isKeyPressed(keyMap_up)) {
		kd_UP = false;
		lf_upPressed = false;
	}
	if (!sf::Keyboard::isKeyPressed(keyMap_down)) {
		kd_DOWN = false;
		lf_downPressed = false;
	}
}