#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <numeric>
#include "GLOBALS.h"
#include "COLPALETTE.h"
class Game;
class FoodManager;

using namespace std;
using namespace sf;


static int foodCount = 0;

class Food
{
public:

	Food(int x, int y, FoodManager* fm);

	void Render(sf::RenderWindow& window);




	void SetPos(int x, int y) {
		xPos = x;
		yPos = y;
		//cout << "Set food pos to X:" << x << " Y:" << y << endl;
	}

	pair<int, int> GetPos() {
		pair<int, int> vec = { xPos, yPos };
		return vec;
	}

	Vector2i GetPosVec() {	//VECTOR VERSION
		return { xPos, yPos };
	}
	
	pair<int, int> GetPos_GRD() {
		pair<int, int> vec = { xPos / GRIDSIZE, yPos / GRIDSIZE};
		return vec;
	}

	void BecomeEaten();

	void MoveDown(int spaces = 1) {
		yPos += GRIDSIZE * spaces;
	}

	//combine these 2
	void BecomeAlive(int value) {
		isNotAlive = false;
		nutrition = value;
		//cout << "FOOD " << id << " BECOME ALIVE" << endl;
		//foodCount++;
		colValue = value;

		switch(value) {
			case (1):
			{
				col = COL_SNAKEBOD1;
				colOutline = COL_SNAKEOUTLINE1;

				nutrition = 1;
				break;
			}
			case (2):
			{
				col = COL_SNAKEBOD2;
				colOutline = COL_SNAKEOUTLINE2;

				nutrition = 1;

				break;
			}
			case (3):
			{
				col = COL_SNAKEBOD3;
				colOutline = COL_SNAKEOUTLINE3;
				nutrition = 1;

				break;
			}
			case (4):
			{
				col = COL_SNAKEBOD4;
				colOutline = COL_SNAKEOUTLINE4;

				nutrition = 1;

				break;
			}
			case (5):
			{
				col = COL_FOOD1;
				colOutline = Color::White;
				nutrition = 2;

				break;
			}
			case (6):
			{
				col = COL_FOOD2;
				colOutline = Color::White;

				nutrition = 3;

				break;
			}
			case (7):
			{
				col = COL_FOOD3;
				colOutline = Color::White;

				nutrition = 4;

				break;
			}
			case (8):
			{
				col = COL_FOOD4;
				colOutline = Color::White;

				nutrition = 5;

				break;
			}
			default:
			{
				col = Color::Magenta;
				break;
			}
		}
	}

	bool GetIsNotAlive() {
		return isNotAlive;
	}

	int getNutrition() {
		return nutrition;
	}

	Color getColour() {
		return col;
	}

	int getColValue() {	//returns int as colour identifier
		return colValue;
	}

	int id = 0;

private:

	int xPos = 0;
	int yPos = 0;

	int nutrition = 1;

	int colValue = 0;

	bool isNotAlive = false;	//PLEASE rename this later and flip all the bools
	Color col;
	Color colOutline;

	sf::CircleShape shape;

	FoodManager* fmRef;


	friend class FoodManager;
};

class FoodManager
{
public:

	FoodManager(Game* game);

	FoodManager(int count = 4) {
		for (size_t i = 0; i < count; i++)
		{
			int x;
			int y;

			

			x = rand() % 31 + 1;
			y = rand() % 23 + 1;

			//alternative method, 2d array of all grid spaces (also use to store positions?) and select randoms from them
			//Food newFood(x * 25, y * 25);
			Food newFood(500, 0 , this);
			allFood.push_back(newFood);
			newFood.isNotAlive = false;
		}
	}

	void RenderAll(sf::RenderWindow& window);

	vector<Food>& getAllFood();

	void SetupFood(int count = 4);

	void Update(float deltaTime);

	bool CoordOverlapsWithSpawnedFood(Vector2i coord);

	int FoodAliveCount() {
		int aliveCount = 0;

		for (Food food : allFood) {
			if (!food.GetIsNotAlive()) {
				aliveCount++;
			}
		}

		return aliveCount;
	}

	void RandomiseNextFoodSpawnTime() {

		cout << "RANDOMISED "<< endl;

		//A pseudo-curve for how long it takes for the next food to spawn depending on how much food is currently present
		switch (FoodAliveCount()) {
			case(0):
			{
				timeUntilNextFoodRand = 0;
				break;
			}
			case(1):
			{
				timeUntilNextFoodRand = 0;
				break;

			}
			case(2):
			{
				timeUntilNextFoodRand = 0;
				break;

			}
			case(3):
			{
				timeUntilNextFoodRand = 0;
				break;

			}
			case(4):
			{
				timeUntilNextFoodRand = 0;
				break;

			}
			case(5):
			{
				timeUntilNextFoodRand = 1;
				break;

			}
			case(6):
			{
				timeUntilNextFoodRand = 1;
				break;

			}
			case(7):
			{
				timeUntilNextFoodRand = 2;
				break;

			}
			case(8):
			{
				timeUntilNextFoodRand = 2;
				break;

			}
			case(9):
			{
				timeUntilNextFoodRand = 3;
				break;

			}
			case(10):
			{
				timeUntilNextFoodRand = 3;
				break;

			}
			case(11):
			{
				timeUntilNextFoodRand = 4;
				break;

			}
			default:
			{
				//timeUntilNextFoodRand = 1;

				timeUntilNextFoodRand = std::numeric_limits<float>::max();
			}
		}

		
	}

	void SetWaterLevel(int location) {
		waterLevel = location;
	}

	

private:

	Game* gameRef;

	vector<Food> allFood;
	int maxFoodCount = MAX_FOOD;
	float foodSpawnDelayMultiplier; //spawns food faster depending on how little is left
	int timeUntilNextFoodBase = 2;
	float timeUntilNextFoodRand = 2;

	float timer = 0; //general tick timer

	float foodSpawnTimer = 0;

	int waterLevel = 0;


	void SpawnFood();

	int GetWaterLevel() {
		return waterLevel;
	}
};

