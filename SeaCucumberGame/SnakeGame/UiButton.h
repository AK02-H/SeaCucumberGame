#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "GLOBALS.h"
#include <assert.h>
#include "FrontEnd.h"
#include "EventSystem.h"

using namespace std;
using namespace sf;


class FrontEnd;

class UiButton
{
public:

	
	UiButton(Vector2i size, FrontEnd* fePtr, int functionId = 0, int additionalParam1 = 0) {

		if (!tex.loadFromFile("Resources/Sprites/buttonDefault.png")) {
			cout << "WARNING: titleLogo.png NOT FOUND" << endl;
			_ASSERTE(true && "BUTTON DEFAULT TEXTURE NOT FOUND"); 
		}
		
		sprite.setTexture(tex);
		sprite.setScale({ size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height});
		feSystem = fePtr;
		feFuncId = functionId;

		additionalParameter_1 = additionalParam1;

	}

	UiButton(FrontEnd* fePtr, Texture &texture, int functionId = 0, int additionalParam1 = 0) {

		sprite.setTexture(texture);
		feSystem = fePtr;
		feFuncId = functionId;

		additionalParameter_1 = additionalParam1;
	}

	/*UiButton(Game* gmPtr, Texture& texture, int functionId = 0, int additionalParam1 = 0) {

		sprite.setTexture(texture);
		gameSystem = gmPtr;
		gmFuncId = functionId;

		additionalParameter_1 = additionalParam1;
	}*/

	void setSprite(Texture& newTex) {
		tex = newTex;
		sprite.setTexture(tex);
	}

	void setScale(Vector2f scale, bool keepPosition = true) {

		if (keepPosition) {
			Vector2f currentPos = sprite.getPosition();
			sprite.setScale(scale);
			setPosition((Vector2i)currentPos);
		}
		else {
			sprite.setScale(scale);
		}

	}

	void flipScaleX() {
		sprite.setScale({ -sprite.getScale().x, sprite.getScale().y });
	}

	void setPosition(Vector2i pos, bool useCenterAsPivot = true) {
		
		if (!useCenterAsPivot) {
			sprite.setPosition((Vector2f)pos);
		}
		else {
			Vector2f newPos = { pos.x - sprite.getLocalBounds().width * sprite.getScale().x / 2, pos.y - sprite.getLocalBounds().height * sprite.getScale().y / 2 };
			sprite.setPosition(newPos);
		}
	}

	void ProcessMouseDownEvent(Vector2i mousePos);

	void Render(RenderWindow &window) {
		window.draw(sprite);
	}

	void setActive(bool isActive) {
		active = isActive;
	}

private:

	Texture tex;	//default texture
	Sprite sprite;

	bool active = true;

	FrontEnd* feSystem;
	Game* gameSystem;
	int feFuncId;
	int gmFuncId;
	int additionalParameter_1;
	
	
};




