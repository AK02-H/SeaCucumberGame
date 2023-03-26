/************************************************/
/*	Sea Cucumber Game	    	                */
/*	Programmer : Niblet, Amin Khan		        */
/*	Date : 11th May, 2022	                    */
/*	Email : akhan2019@elam.co.uk                */
/* 	Webpages : https://niblet.itch.io/		    */
/*             https://github.com/Niblet-0		*/
/* 	Discord : Niblet#7617		                */
/* 	                    		                */
/* 	Made for Teesside university		        */
/* 	Uhh, don't redistribute source code         */
/* 	and other copyright stuff                   */
/* 	                                            */
/************************************************/

#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "GLOBALS.h"
#include "COLPALETTE.h"
#include "LList.h"
#include "Water.h"
#include "Food.h"
#include "TextManager.h"
#include "AudioManager.h"
#include "Collider.h"
#include "Input.h"

enum Direction{N, E, S, W};

using namespace std;
using namespace sf;


class Segment {

public:

	Segment(Segment* parentSegment = nullptr, size_t id = 0) {
		if (parentSegment == nullptr) {
			//is the first segment created
		}
		else {
			next = parentSegment;
		}

		//ID = id;
		squareShape.setOutlineThickness(SNAKE_OUTLINETHICKNESS);
		squareShape.setSize({ GRIDSIZE, GRIDSIZE });
	}

	~Segment() {
		next = nullptr;
	}

	void Render(sf::RenderWindow& window) {
		squareShape.setPosition({ xPos, yPos });
		window.draw(squareShape);

	}

	void Move() {

		if (isHead) {
			xPos += 25;
		}
		else {
			pair<int, int> newPos = next->GetPos();
			xPos = newPos.first;
			yPos = newPos.second;
		}

		
	}

	void Move(int x, int y) {

	}

	pair<int, int> GetPos() {
		pair<int, int> vec = { xPos, yPos };
		return vec;
	}

	pair<int, int> GetGridPos() {
		pair<int, int> vec = {(int) (xPos / GRIDSIZE), (int) (yPos / GRIDSIZE) };
		return vec;
	}

	Segment* GetNext() {
		return next;
	}

	void SetPos(int x, int y) {

		xPos = x;
		yPos = y;
	}

	void SetIsHead(bool value) {
		isHead = value;
	}

	void Clear() {

	}

	int GetID() {

		return ID;
	}

	void SetID(int value) {
		ID = value;
	}

	

	void setColours(Color body, Color outline) {
		bodyCol = body;
		outlineCol = outline;

		squareShape.setFillColor(bodyCol);
		squareShape.setOutlineColor(outlineCol);
	}

	void setColourSet(int playerId) {

		switch (playerId)
		{
		case(0):
		{
			bodyCol = COL_SNAKEBOD1;
			outlineCol = COL_SNAKEOUTLINE1;

			break;
		}
		case(1):
		{
			bodyCol = COL_SNAKEBOD2;
			outlineCol = COL_SNAKEOUTLINE2;

			break;

		}
		case(2):
		{
			bodyCol = COL_SNAKEBOD3;
			outlineCol = COL_SNAKEOUTLINE3;

			break;

		}
		case(3):
		{
			bodyCol = COL_SNAKEBOD4;
			outlineCol = COL_SNAKEOUTLINE4;

			break;

		}
		default:
		{
			bodyCol = COL_SNAKEBOD1;
			outlineCol = COL_SNAKEOUTLINE1;

			break;
		}
		}

		squareShape.setFillColor(bodyCol);
		squareShape.setOutlineColor(outlineCol);


	}


private:

	int ID = 0;
	Color bodyCol = COL_SNAKEBOD1;
	Color outlineCol = COL_SNAKEOUTLINE1;

	bool isHead = false;

	float xPos = 100;
	float yPos = 100;

	Segment* next;

	sf::RectangleShape squareShape;


};

using namespace std;
class Snake : public Collider
{

public:

	Snake(int spawnX = 5, int spawnY = 4, int id = 0);

	void Render(sf::RenderWindow &window) {

		if (!isAlive) return;

		LListNode<Segment>* loopingNode = (snakeSegments.front());
		//LListNode<Segment>* loopingNode = (snakeSegments.front());

		while (loopingNode != nullptr) {

			

			loopingNode->getElement().Render(window);
			//loopingNode->Render(window);
			loopingNode = loopingNode->getNext();
		}

		window.draw(oxBar);

		if (curOxygen <= 0) {
			window.draw(drownWarning);
		}
	}

	void Update(float deltaTime);

	virtual void HandleInput() {

		//when not submerged in water:
		/*
		* if facing upwards
		* 
		* only allow left, right and down inputs
		* if travels sideways for more than 3 ticks, force direction downwards
		*/

		if (useBufferNextTick) return;
		if (hasReceivedInputThisTick_Buffer) return;
		if (hasReceivedInputThisTick) {	//add input buffer feature
			
			

			if (INPUT.kd_LEFT) {
				if (direction != Direction::E) {
					inputBuffer = Direction::W;
					cout << "EXTRA INPUT" << endl;
					hasReceivedInputThisTick_Buffer = true;
				}
			}
			else if (INPUT.kd_RIGHT) {
				if (direction != Direction::W) {
					inputBuffer = Direction::E;
					cout << "EXTRA INPUT" << endl;
					hasReceivedInputThisTick_Buffer = true;
				}
			}
			else if (INPUT.kd_UP) {
				if (direction != Direction::S && isSubmerged) {	//don't allow this input if unsubmerged
					inputBuffer = Direction::N;
					cout << "EXTRA INPUT" << endl;
					hasReceivedInputThisTick_Buffer = true;
				}
			}
			else if (INPUT.kd_DOWN) {
				if (direction != Direction::N) {
					inputBuffer = Direction::S;
					cout << "EXTRA INPUT" << endl;
					hasReceivedInputThisTick_Buffer = true;
				}
			}

			return;		
		}


		
		if (INPUT.kd_LEFT) {
			if (direction != Direction::E) {
				direction = Direction::W;
				prevHorizontalDirection = Direction::W;

				hasReceivedInputThisTick = true;
			}
		}
		else if (INPUT.kd_RIGHT) {
			if (direction != Direction::W) {
				direction = Direction::E;
				prevHorizontalDirection = Direction::E;

				hasReceivedInputThisTick = true;
			}
		}
		else if (INPUT.kd_UP) {
			if (direction != Direction::S && isSubmerged) {	//don't allow this input if unsubmerged
				direction = Direction::N;

				hasReceivedInputThisTick = true;
			}
		}
		else if (INPUT.kd_DOWN) {
			if (direction != Direction::N) {
				direction = Direction::S;

				hasReceivedInputThisTick = true;
			}
		}

		
	}

	#pragma region getters and setters



	void AssignFoodManager(FoodManager* fm) {
		foodManager = fm;
	}

	void AssignTextManager(TextManager* tm) {
		textManagerRef = tm;
	}

	void AssignAudioManager(AudioManager* am) {
		audioManager = am;
	}

	

	void SetIsSubmerged(int waterSurface) {


		if (headPos().y >= waterSurface) {
			isSubmerged = true;
		}
		else {
			isSubmerged = false;
		}

		//set variable to save for later (currently only used in AI)
		waterSurfacePos = waterSurface * GRIDSIZE;
	}

	

	pair<int, int> getHeadPos() {
		return snakeSegments.getFront().GetPos();
	}

	Vector2i getHeadVec() {	//VECTOR VERSION	
		return { snakeSegments.getFront().GetPos().first, snakeSegments.getFront().GetPos().second };
	}



	void AssignOtherSnakeRef(Snake* snk) {
		otherSnakes.push_back(snk);
	}

	void Die() {
		cout << "DIE" << endl;
		audioManager->play_sfx_die();
		isAlive = false;
	}

	bool getIsAlive() {
		return isAlive;
	}

	Direction getDirection() {
		return direction;
	}

	int getScore() {
		return score;
	}

	void setIsAlive(bool alive) {
		isAlive = alive;
	}

	float getOxygenPercentage() {
		return (float)curOxygen / maxOxygen;
	}

	void setIsAI(bool is) {
		isAI = is;
	}

	void setSuperRespiration(bool is) {
		superRespiration = is;
	}

	void setMaxOxygenMultiplier(int multiplier) {
		maxOxygen *= multiplier;
		curOxygen *= multiplier;
	}

	int getPlayerId() {
		return playerID;
	}
	

#pragma endregion

	bool CoordWithinSnake(Vector2i coord) {

		LListNode<Segment>* loopingNode = snakeSegments.front();

		pair<int, int> pairConversion;
		pairConversion.first = coord.x;
		pairConversion.second = coord.y;

		do {
			if (loopingNode->getElement().GetPos() == pairConversion) {
				return true;
			}

			loopingNode = loopingNode->getNext();
		} while (loopingNode->getNext() != nullptr);

		return false;


	}

	vector<FloatRect> CollisionPoints() override;
	bool DoesCollide(Collider* collider) override;
	/*bool DoesCollide(FloatRect simulated, Collider* collider) override;*/


protected:

	bool superRespiration = false;

	LList<Segment> snakeSegments;

	Direction direction;
	Direction prevHorizontalDirection = Direction::E;

	bool isSubmerged = false;

	int curOxygen = MAX_OXYGEN;
	int maxOxygen = MAX_OXYGEN;

	int waterSurfacePos = 0;	//IN PIXELS

	vector<Snake*> otherSnakes;
	FoodManager* foodManager;

	float timer = 0;

	bool isAI = false;


	FloatRect headRect() {

		pair<int, int> headPosition = snakeSegments.front()->getElementPtr()->GetPos();


		//Food collision


		FloatRect headRect = FloatRect({ (float)headPosition.first, (float)headPosition.second }, { GRIDSIZE, GRIDSIZE });

		return headRect;

	}

	Vector2i headPos() {
		pair<int, int> headPosition = snakeSegments.front()->getElementPtr()->GetGridPos();
		return{ headPosition.first, headPosition.second };

	}

	bool AABB(sf::FloatRect b1, sf::FloatRect b2) {	//make a macro?
		return (b2.left < b1.left + b1.width) && (b2.left + b2.width > b1.left) &&
			(b2.top < b1.top + b1.height) && (b2.top + b2.height > b1.top);


	}


	

private:

	Input INPUT;	//should this be a pointer?

	int playerID = 0;
	Color bodyCol;

	bool isAlive = true;

	int length;	//used?
	int score = 0;

	

	//HUD
	RectangleShape oxBar;
	CircleShape drownWarning;

	int airTime_H = 0;
	int airTime_V = 0;
	//vector<Segment> segments;

	const int kMaxDrowningTicks = 3;
	int drowningCounter = 0;
	int ticksSinceEmerge = 0;
	int ticksSinceSubmerge = 0;



	Direction inputBuffer;
	bool hasReceivedInputThisTick = false;
	bool hasReceivedInputThisTick_Buffer = false;
	bool useBufferNextTick = false;

	bool tempBool = false;



	float lastTick;

	Vector2i previousPosition;


	TextManager* textManagerRef;
	AudioManager* audioManager;

	

	void ModifyMoveValue(pair<int, int>& coord, bool overrideWithBufferValue = false) {
		
		if (!overrideWithBufferValue) {
			switch (direction)
			{
			case N:
				//std::cout << "NORTH" << std::endl;
				coord.second -= GRIDSIZE;
				break;

			case E:
				//std::cout << "EAST" << std::endl;
				coord.first += GRIDSIZE;
				break;

			case S:
				if (direction != Direction::N) {
					//std::cout << "SOUTH" << std::endl;
					coord.second += GRIDSIZE;
				}

				break;
			case W:
				if (direction != Direction::E) {
					//std::cout << "WEST" << std::endl;
					coord.first -= GRIDSIZE;
				}

				break;
			default:
				break;
			}
		}
		else {
			switch (inputBuffer)
			{
			case N:
				//std::cout << "NORTH" << std::endl;
				coord.second -= GRIDSIZE;
				break;

			case E:
				//std::cout << "EAST" << std::endl;
				coord.first += GRIDSIZE;
				break;

			case S:
				if (direction != Direction::N) {
					//std::cout << "SOUTH" << std::endl;
					coord.second += GRIDSIZE;
				}

				break;
			case W:
				if (direction != Direction::E) {
					//std::cout << "WEST" << std::endl;
					coord.first -= GRIDSIZE;
				}

				break;
			default:
				break;
			}
		}
		

		//cout << "new position is " << coord.first << " " << coord.second << endl;
	}

	

	


	

};


class SnakeHUD
{
public:
	SnakeHUD(Snake* snakePtr, int snakeId = 0) {
		snake = snakePtr;
		id = snakeId;

		if (!font_1.loadFromFile("Resources/Fonts/f_vcr.ttf")) {
			_ASSERTE("FONT NOT FOUND");
		}
		else {
			cout << "VCR Font loaded" << endl;
		}

		playerScore.setFont(font_1);
		playerScore.setString("P" + std::to_string(id + 1) + ": ");
		
		playerScore.setPosition({ SCREEN_W + SCOREBOARD_W / 2 - kOxygenBarMaxSize / 2, (float)(kVertDistribution - 10 + kVertDistribution * snakeId) });

		switch (snakeId)
		{
		case(0):
			playerScore.setColor(COL_SNAKEBOD1);
			break;
		case(1):
			playerScore.setColor(COL_SNAKEBOD2);
			break;
		case(2):
			playerScore.setColor(COL_SNAKEBOD3);
			break;
		case(3):
			playerScore.setColor(COL_SNAKEBOD4);
			break;
		default:
			playerScore.setColor(COL_SNAKEBOD1);
			break;

		}

		oxygenBar.setSize({ (float)kOxygenBarMaxSize, 40 });
		oxygenBar.setPosition({ SCREEN_W + SCOREBOARD_W / 2 - kOxygenBarMaxSize / 2, (float)(kVertDistribution + 30 + kVertDistribution * snakeId) });
		oxygenBar.setFillColor(Color::Cyan);

		oxygenBarBackdrop.setSize({ (float)kOxygenBarMaxSize, 40 });
		oxygenBarBackdrop.setPosition({ (float)((SCREEN_W + SCOREBOARD_W / 2 - kOxygenBarMaxSize / 2) - oxygenBar.getSize().x * 0.035), (float)((kVertDistribution + 30 + kVertDistribution * snakeId) - oxygenBar.getSize().y * 0.15) });
		oxygenBarBackdrop.setScale({ 1.07, 1.3 });
		oxygenBarBackdrop.setFillColor(COL_SCOREBOARDGREEN);
		//oxygenBarBackdrop.setFillColor(Color::Black);
	}

	//Copy constructor (for when pushed into a vector)
	SnakeHUD(const SnakeHUD& old) {
		cout << "SNAKE HUD COPY CONSTRUCTOR CALLED" << endl;

		snake = old.snake;
		id = old.id;
		if (!font_1.loadFromFile("Resources/Fonts/f_vcr.ttf")) {
			_ASSERTE("FONT NOT FOUND");
		}
		else {
			cout << "VCR Font loaded" << endl;
		}

		playerScore.setFont(font_1);
		playerScore.setString("P" + std::to_string(id + 1) + ": ");

		playerScore.setPosition({ SCREEN_W + SCOREBOARD_W / 2 - kOxygenBarMaxSize / 2, (float)(kVertDistribution - 10 + kVertDistribution * id) });

		switch (id)
		{
		case(0):
			playerScore.setColor(COL_SNAKEBOD1);
			break;
		case(1):
			playerScore.setColor(COL_SNAKEBOD2);
			break;
		case(2):
			playerScore.setColor(COL_SNAKEBOD3);
			break;
		case(3):
			playerScore.setColor(COL_SNAKEBOD4);
			break;
		default:
			playerScore.setColor(COL_SNAKEBOD1);
			break;

		}

		oxygenBar.setSize({ (float)kOxygenBarMaxSize, 40 });
		oxygenBar.setPosition({ SCREEN_W + SCOREBOARD_W / 2 - kOxygenBarMaxSize / 2, (float)(kVertDistribution + 30 + kVertDistribution * id) });
		oxygenBar.setFillColor(Color::Cyan);

		oxygenBarBackdrop.setSize({ (float)kOxygenBarMaxSize, 40 });
		oxygenBarBackdrop.setPosition({ (float)((SCREEN_W + SCOREBOARD_W / 2 - kOxygenBarMaxSize / 2) - oxygenBar.getSize().x * 0.035), (float)((kVertDistribution + 30 + kVertDistribution * id) - oxygenBar.getSize().y * 0.15) });
		oxygenBarBackdrop.setScale({ 1.07, 1.3 });
		oxygenBarBackdrop.setFillColor(COL_SCOREBOARDGREEN);
	}

	void UpdateData() {

		if (snakeIsDead) return;

		if (!snake->getIsAlive()) {

			snakeIsDead = true;
			SetDead();
			oxygenBar.setFillColor(Color::Red);
			return;
		}

		float oxygen = snake->getOxygenPercentage();
		oxygenBar.setScale({ oxygen, 1 });
		
		string s = "P" + std::to_string(id + 1) + ":    ";

		char str[5];
		snprintf(str, 5, "%04d", snake->getScore());

		playerScore.setString(s.append(str));
	}

	void SetDead() {
		playerScore.setColor(Color::Red);
	}


	void Render(RenderWindow& window) {
		window.draw(oxygenBarBackdrop);
		window.draw(oxygenBar);
		window.draw(playerScore);
	}


private:

	const int kVertDistribution = (int) (SCREEN_H / 5);

	Snake* snake;
	int id = 0;

	bool snakeIsDead = false;

	Font font_1;
	Text playerScore;

	RectangleShape oxygenBar;
	RectangleShape oxygenBarBackdrop;

	const float kOxygenBarMaxSize = SCOREBOARD_W * 0.7f;

	stringstream ss;

};





