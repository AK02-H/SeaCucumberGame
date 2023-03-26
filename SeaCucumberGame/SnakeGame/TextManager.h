#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <vector>
#include "COLPALETTE.h"

using namespace std;
using namespace sf;

class TxtPopup
{
public:

	TxtPopup(sf::Font* font) {
		//this->font = font;
		text.setFont(*font);
		text.setOutlineColor(Color::White);
		text.setOutlineThickness(1);


	}

	void SetupPopupText(string label = "TEXT", int size = 15, Color col = Color::Magenta) {
		text.setString(label);
		text.setCharacterSize(size);
		text.setColor(col);
	}

	void SetPopupPosition(Vector2i pos) {
		text.setPosition((Vector2f)pos);
	}

	void LaunchText() {
		opacity = 1;
		isInUse = true;
	}

	bool getIsInUse() {
		return isInUse;
	}

	void setIsInUse(bool is) {
		isInUse = is;
	}

	void Update(float dt) {

		if (isInUse) {
			text.setPosition(text.getPosition() - Vector2f(0, dt * moveSpeed));

			opacity -= (dt * fadeSpeed);

			Color currentCol = text.getColor();
			Color newCol(currentCol.r, currentCol.g, currentCol.b, currentCol.a);

			text.setColor(newCol);
			if (opacity <= 0) {
				isInUse = false;
			}
		}

	}

	void Render(RenderWindow &window) {
		if (isInUse) {
			window.draw(text);
		}
	}

private:

	bool isInUse = false;
	//Font* font;
	Text text;
	RectangleShape testShape;
	float moveSpeed = 18;
	float fadeSpeed = 1;

	float opacity = 1;
};

class TextManager
{

public:

	TextManager() {
		

		if (!font_1.loadFromFile("Resources/Fonts/f_vcr.ttf")) {
			_ASSERTE("FONT NOT FOUND");
		}
		else {
			cout << "VCR Font loaded" << endl;
		}

		for (size_t i = 0; i < poolSize; i++)
		{
			//TxtPopup newText(&font);
			TxtPopup newText(&font_1);
			textObjects.push_back(newText);
			
		}

	}

	void Update(float dt) {
		for (TxtPopup& txt : textObjects) {
			txt.Update(dt);
		}
	}

	void Render(RenderWindow &window) {
		for (TxtPopup &txt : textObjects) {
			txt.Render(window);
		}
	}



	void PlaceTextPopup_Food(int scoreValue, int colValue, Vector2i position) {
		
		Color col;

		switch (colValue) {
			case (1):
			{
				col = COL_SNAKEBOD1;
				break;
			}
			case (2):
			{
				col = COL_SNAKEBOD2;
				break;
			}
			case (3):
			{
				col = COL_SNAKEBOD3;
				break;
			}
			case (4):
			{
				col = COL_SNAKEBOD4;
				break;
			}
			case (5):
			{
				col = COL_FOOD1;
				break;
			}
			case (6):
			{
				col = COL_FOOD2;
				break;
			}
			case (7):
			{
				col = COL_FOOD3;
				break;
			}
			case (8):
			{
				col = COL_FOOD4;
				break;
			}
			default:
			{
				col = Color::Magenta;
				break;
			}
		}

		for (TxtPopup &txt : textObjects) {
			if (!txt.getIsInUse()) {
				txt.SetupPopupText(to_string(scoreValue), 30, col);
				txt.SetPopupPosition(position);
				txt.LaunchText();
				return;
			}
		}
		cout << "WARNING: NO TEXT OBJECTS LEFT IN THE POOL" << endl;;
		_ASSERT(false && "NO TEXT OBJECTS LEFT IN THE POOL");
	}


private:

	Font font_1;

	vector<TxtPopup> textObjects;
	int poolSize = 10;
};





