#include "Food.h"
#include "Game.h"
#include "GLOBALS.h"

#pragma region Food


Food::Food(int x, int y, FoodManager* fm) {
	xPos = x;
	yPos = y;
	
	id = foodCount;

	foodCount++;

	fmRef = fm;


	nutrition = rand() % 7 + 1;
	colValue = nutrition;
	//Random from 1 to 8 (inclusive)
	//1-4 snake colours, all worth 1
	//5-8 regular, worth 2 - 5

	shape.setOutlineThickness(2);
	shape.setOutlineColor(Color::White);

	switch (nutrition) {
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
		nutrition = 1;
		break;
	}
	}
}


void Food::Render(sf::RenderWindow& window) {

	shape.setFillColor(col);
	shape.setOutlineColor(colOutline);
	shape.setRadius(GRIDSIZE / 2);
	shape.setPosition({ (float)xPos, (float)yPos });
	window.draw(shape);
}


void Food::BecomeEaten() {
	isNotAlive = true;
	fmRef->RandomiseNextFoodSpawnTime();
	//foodCount--;
}

#pragma endregion


#pragma region FoodManager

FoodManager::FoodManager(Game* game) {
	gameRef = game;
}

void FoodManager::RenderAll(sf::RenderWindow& window) {
	for (Food foodItem : allFood) {
		if (!foodItem.GetIsNotAlive()) {
			foodItem.Render(window);

			//cout << "Rendering " << foodItem.id << endl;
		}
	}
}

vector<Food>& FoodManager::getAllFood() {
	return allFood;
}

void FoodManager::SetupFood(int count) {
	for (size_t i = 0; i < count; i++)
	{
		int x = rand() % 31 + 1;
		int y = rand() % 23 + 1;

		while (!gameRef->SpaceIsAvailable({ x, y })) {	// do check so it doesn't get stuck on an infinite loop. maybe limit how high the count can be
			x = rand() % 31 + 1;
			y = rand() % 23 + 1;
		}



		//alternative method, 2d array of all grid spaces (also use to store positions?) and select randoms from them
		Food newFood(x * GRIDSIZE, y * GRIDSIZE, this);
		//Food newFood(500, 0);
		allFood.push_back(newFood);
		newFood.isNotAlive = false;
	}
}

void FoodManager::Update(float deltaTime) {
	

	timer += deltaTime;

	if (timer > TICK * 2) {
		for (Food& food : allFood) {
			if (!food.GetIsNotAlive()) {
				if (food.GetPos_GRD().second < waterLevel) {
					food.MoveDown();
				}
			}
		}

		timer = 0;
	}


	int aliveCount = 0;

	for (Food food : allFood) {
		if (!food.GetIsNotAlive()) {
			aliveCount++;
		}
	}



	if (aliveCount < maxFoodCount) {




		foodSpawnTimer += deltaTime;


		if (foodSpawnTimer >= timeUntilNextFoodRand) {
			SpawnFood();

			RandomiseNextFoodSpawnTime();
			foodSpawnTimer = 0;
			cout << "SPAWNED FOOD" << endl;
		}

	}

	//cout << "FOOD COUNT: " << foodCount << endl;
}

bool FoodManager::CoordOverlapsWithSpawnedFood(Vector2i coord) {
	for (Food food : allFood) {
		if (!food.GetIsNotAlive()) {
			pair<int, int> foodPos = food.GetPos();
			if (foodPos.first == coord.x && foodPos.second == coord.y) {
				return true;
			}
		}
	}

	return false;
}



void FoodManager::SpawnFood() {

	int x = rand() % ((SCREEN_W / GRIDSIZE) - 1) + 1;
	//int y = rand() % ((SCREEN_H / GRIDSIZE)) + 1;	//add -1??	//spawning food everywhere

	int newValue = rand() % 7 + 1;

	//For only spawning food in water
	int y = (rand() % (SCREEN_H - (gameRef->waterRef()->GetSurfaceLoc() - GRIDSIZE * 4))) + (gameRef->waterRef()->GetSurfaceLoc() - GRIDSIZE * 4);
	y /= GRIDSIZE;


	while (!gameRef->SpaceIsAvailable({ x, y })) {	// do check so it doesn't get stuck on an infinite loop. maybe limit how high the count can be
		x = (rand() % 31 + 1);
		int y = rand() % 24 + 1;

		
	}

	for (Food& food : allFood) {
		if (food.GetIsNotAlive()) {

			food.SetPos(x * GRIDSIZE, y * GRIDSIZE);
			food.BecomeAlive(newValue);
			cout << "BECOME ALIVE" << endl;
			break;

		}
	}

}

#pragma endregion


