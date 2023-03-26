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

#include "Snake.h"
#include "GLOBALS.h"


Snake::Snake(int spawnX, int spawnY, int id)
{
	cout << "SNAKE CONSTRUCTED" << endl;

	playerID = id;

	INPUT = Input(playerID);

	#pragma region Calculate spawn positions

	int xDirOffset = 1;
	int yDirOffset = 1;

	int spawnX1 = spawnX;
	int spawnY1 = spawnY;
	int spawnX2 = spawnX;
	int spawnY2 = spawnY;
	int spawnX3 = spawnX;
	int spawnY3 = spawnY;

	switch (playerID)
	{
	case(0):
	{
		direction = Direction::E;
		xDirOffset = 1;
		yDirOffset = 0;


		spawnX2 = spawnX - 1;
		spawnX3 = spawnX - 2;

		bodyCol = COL_SNAKEBOD1;

		break;
	}
	case(1):
	{
		direction = Direction::W;
		xDirOffset = -1;
		yDirOffset = 0;

		spawnX2 = spawnX + 1;
		spawnX3 = spawnX + 2;

		bodyCol = COL_SNAKEBOD2;


		break;

	}
	case(2):
	{
		direction = Direction::E;
		xDirOffset = 0;
		yDirOffset = 1;

		spawnY2 = spawnY - 1;
		spawnY3 = spawnY - 2;

		bodyCol = COL_SNAKEBOD3;


		break;

	}
	case(3):
	{
		direction = Direction::W;
		xDirOffset = 0;
		yDirOffset = -1;

		spawnY2 = spawnY + 1;
		spawnY3 = spawnY + 2;

		bodyCol = COL_SNAKEBOD4;


		break;

	}
	default:
	{
		direction = Direction::E;
		xDirOffset = 1;
		yDirOffset = 0;

		spawnX2 = spawnX - 1;
		spawnX3 = spawnX - 2;

		bodyCol = COL_SNAKEBOD1;

		break;
	}
	}
#pragma endregion

	#pragma region Create initial snake segments


	Segment* firstSegment = new Segment();
	firstSegment->SetIsHead(true);

	//LListNode<Segment> firstNode(firstSegment, nullptr);
	firstSegment->SetPos(GRIDSIZE * spawnX1, GRIDSIZE * spawnY1);
	firstSegment->setColourSet(playerID);
	snakeSegments.addFront(*firstSegment);


	Segment* secondSegment = new Segment(firstSegment);
	secondSegment->SetPos(GRIDSIZE * spawnX2, GRIDSIZE * spawnY2);
	secondSegment->setColourSet(playerID);
	snakeSegments.addBack(*secondSegment);

	Segment* thirdSegment = new Segment(secondSegment);
	thirdSegment->SetPos(GRIDSIZE * spawnX3, GRIDSIZE * spawnY3);	//????????
	thirdSegment->setColourSet(playerID);
	snakeSegments.addBack(*thirdSegment);

	thirdSegment->SetID(3);

	#pragma endregion

	#pragma region Create HUD

	//Init oxygen bar hud
	oxBar.setFillColor(Color::Cyan);
	oxBar.setSize({ GRIDSIZE , GRIDSIZE / 6 });
	oxBar.setPosition(GRIDSIZE * (spawnX - 1 * xDirOffset), GRIDSIZE * (spawnY - 0 * yDirOffset));
	oxBar.setOutlineThickness(1);
	oxBar.setOutlineColor(Color::Black);

	drownWarning.setFillColor(Color::Red);
	drownWarning.setOutlineColor(Color::Black);
	drownWarning.setOutlineThickness(1);
	drownWarning.setRadius(GRIDSIZE / 2.5);
	drownWarning.setPosition(GRIDSIZE * (spawnX - 1 * xDirOffset), GRIDSIZE * (spawnY - 0 * yDirOffset));

	#pragma endregion


}

vector<FloatRect> Snake::CollisionPoints() {
	//GETS BODY AND NOT HEAD
	vector<FloatRect> gatherColPoints;
	LListNode<Segment>* loopingNode = snakeSegments.front();

	loopingNode = loopingNode->getNext();

	while (loopingNode != nullptr)
	{


		pair<int, int> pairConversion = loopingNode->getElement().GetPos();
		FloatRect newRect({ (float)pairConversion.first, (float)pairConversion.second }, { 25, 25 });
		gatherColPoints.push_back(newRect);

		loopingNode = loopingNode->getNext();
	} 


	return gatherColPoints;
}

bool Snake::DoesCollide(Collider* collider) {
	vector<FloatRect> rectsToCheckAgainst = collider->CollisionPoints();
	FloatRect headRectangle = headRect();

	for (FloatRect rect : rectsToCheckAgainst) {
		if (AABB(headRectangle, rect)) return true;
	}

	return false;
}



void Snake::Update(float deltaTime) {

	if (!isAlive) return;

	if(!isAI) INPUT.Update();

	timer += deltaTime;

	HandleInput();

	if (timer >= TICK) {

		#pragma region Handle water properties

		//Handle water submerging
		
		if (!isSubmerged) {

			curOxygen = maxOxygen;

			if (direction == Direction::E || direction == Direction::W) {
				airTime_H++;
				if (airTime_H >= 2) {
					//force downwards
					cout << "FORCE DOWNWARDS" << endl;
					direction = Direction::S;
					airTime_H = 0;
				}
			}
			else {
				airTime_H = 0;
			}


			if (direction == Direction::N) {
				airTime_V++;
				if (airTime_V >= 3) {
					//force to previous horiz position
					cout << "FORCE SIDEWAYS" << endl;
					direction = prevHorizontalDirection;
					airTime_V = 0;
				}
			}
			else {
				airTime_V = 0;
			}

			if (ticksSinceEmerge == 0) {
				audioManager->play_sfx_oxygen();
			}


			ticksSinceSubmerge = 0;
			ticksSinceEmerge++;

		}
		else {


			airTime_H = 0;
			airTime_V = 0;

			if (ticksSinceSubmerge == 0) {
				audioManager->play_sfx_splash();
			}

			ticksSinceSubmerge++;
			ticksSinceEmerge = 0;

			
		}

		#pragma endregion

		#pragma region Input and movement

		pair<int, int> newPos = snakeSegments.front()->getElementPtr()->GetPos();
		pair<int, int> hudPos = { oxBar.getPosition().x, oxBar.getPosition().y };

		//Calculates input and input buffer

		bool shouldUseBuffer = useBufferNextTick;

		if (hasReceivedInputThisTick_Buffer) {
			useBufferNextTick = true;
			hasReceivedInputThisTick_Buffer = false;
		}
		
		if (useBufferNextTick) {
			cout << "USE STORED BUFFER DIRECTION" << endl;
			useBufferNextTick = false;
		}

		ModifyMoveValue(newPos, shouldUseBuffer);
		ModifyMoveValue(hudPos, shouldUseBuffer);

		snakeSegments.moveBackToFront();
		snakeSegments.front()->getElementPtr()->SetPos(newPos.first, newPos.second);

		oxBar.setPosition(hudPos.first, hudPos.second);
		drownWarning.setPosition(hudPos.first, hudPos.second);

		#pragma endregion

		#pragma region Oxygen

		//OXYGEN

		if (curOxygen <= 0) {
			//DIE
			cout << "OUT OF O2" << endl;

			drowningCounter++;

			if (drowningCounter > kMaxDrowningTicks) {

				score--;

				if (score < 1) {
					
					Die();
					return;
				}

				snakeSegments.removeBack();
				audioManager->play_sfx_damage();
				score = snakeSegments.count();
				drowningCounter = 0;

				
			}
		}
		else {
			curOxygen--;
		}


		oxBar.setScale({(float) curOxygen / maxOxygen, 1 });

		#pragma endregion


		//ALL COLLISION

		#pragma region Collisions

		#pragma region Food collision


		//FOOD COLLISION///

		FloatRect headRectangle = headRect();

		for (Food& i : foodManager->getAllFood()) {

			pair<int, int> foodPosition = i.GetPos();
			FloatRect foodRect = FloatRect({ (float)foodPosition.first, (float)foodPosition.second }, { GRIDSIZE, GRIDSIZE });

			bool hasCollided = AABB(headRectangle, foodRect);

			if (hasCollided) {

				if (!i.GetIsNotAlive()) {
					cout << "FOOD COLLISION" << endl;
					pair<int, int> snakeHead = snakeSegments.front()->getElementPtr()->GetPos();

					int foodValue = i.getNutrition();
					int foodColValue = i.getColValue();

					//If eating food of the same colour as body, gain a bonus
					if (i.getColour() == bodyCol) {
						foodValue = 4;
						audioManager->play_sfx_eatBonus();
					}


					textManagerRef->PlaceTextPopup_Food(foodValue, foodColValue, { snakeHead.first, snakeHead.second });
					Segment* last = snakeSegments.back()->getElementPtr();
					Segment newSegment(last);
					pair<int, int> backPos = snakeSegments.back()->getElementPtr()->GetPos();

					
					for (size_t i = 0; i < foodValue; i++)
					{
						newSegment.SetPos(backPos.first, backPos.second);
						newSegment.setColourSet(playerID);
						snakeSegments.addBack(newSegment);

					}

					audioManager->play_sfx_eat();
					i.BecomeEaten();
					score = snakeSegments.count();
					
				}


			}
		}

#pragma endregion

		#pragma region Snake collision


		//Self Collision

		if (DoesCollide(this)) {
			cout << "DEATH BY COLLIDING WITH SELF" << endl;

			Die();
		}



		//Other snakes collision (body)

		for (Snake* snake : otherSnakes) {
			if (snake->getIsAlive()) {
				if (DoesCollide(snake)) {
					cout << "DEATH BY COLLIDING WITH OTHER SNAKE" << endl;
					Die();
				}
			}
		}

		//Other snakes collision (head)
		for (Snake* snake : otherSnakes) {
			if (snake->getIsAlive()) {

				pair<int, int> otherHeadPos = snake->getHeadPos();
				pair<int, int> thisHeadPos = getHeadPos();

				if (thisHeadPos == otherHeadPos) {

					cout << "COLLIDED WITH HEAD OF OTHER SNAKE" << endl;

					snake->Die();
					Die();

					
				}

			}
		}

		#pragma endregion

		#pragma region Wall collision, but the word collision being used very loosely

		//Wall Collision (Should top of screen be excluded?)
		Vector2i headPosition = headPos();
		if (headPosition.x < 0 || headPosition.x >= SCREEN_W / GRIDSIZE || headPosition.y >= SCREEN_H / GRIDSIZE) {
			Die();
		}

		#pragma endregion

		#pragma endregion

		hasReceivedInputThisTick = false;
		if (shouldUseBuffer) useBufferNextTick = false;
		timer = 0;
	}


	







}