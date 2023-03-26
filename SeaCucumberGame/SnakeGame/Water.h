#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include "COLPALETTE.h"
#include "LList.h"
#include "Food.h"
#include "Collider.h"

using namespace std;
using namespace sf;


class Water
{
public:


	Water() {
		squareShape.setFillColor(COL_BLUE);
		squareShape.setOutlineThickness(6);
		squareShape.setSize({ SCREEN_W, SCREEN_H });
		squareShape.setPosition({ 0, (float) 0 + getLiteralDistance()});				//set to middle of screen
		
	}
	


	void Render(sf::RenderWindow& window) {
		squareShape.setPosition({ 0, (float)0 + getLiteralDistance() });
		window.draw(squareShape);

	}

	void Update(float deltaTime) {

		timer += deltaTime;
		
		//FORMULA FOR DECREASE RATE BASED OFF SCREEN HEIGHT:
		// 90 / TICK = Tx (number of ticks in a game)
		// scrnH / GRIDSPACE = H (number of number of spaces the water can move down)
		// Need to decrease H spaces within Tx
		//Tick multiplier should be H / Tx

		if (timer >= TICK * ((int)(((GAMELENGTH + 1) / TICK) / (SCREEN_H / GRIDSIZE)))) {
			gridDistanceFromTop++;
			timer = 0;
		}

	}

	int GetSurfaceLoc() {
		return getLiteralDistance();
	}
	
	int GetSurfaceLoc_GRD() {
		return gridDistanceFromTop;
	}

private:

	int gridDistanceFromTop = 0; //in grid spaces
	float timer = 0;
	sf::RectangleShape squareShape;

	int getLiteralDistance() {
		return GRIDSIZE * gridDistanceFromTop;
	}

};

