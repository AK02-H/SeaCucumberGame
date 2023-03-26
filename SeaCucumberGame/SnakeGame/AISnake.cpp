#include "AISnake.h"

void AISnake::HandleInput(){	//Replaces player input with a state machie

	//Only do this on a tick
	if (timer < TICK) return;


	if (!isSubmerged) {
		//direction = prevHorizontalDirection;
	}


	//aim is to set direction
	Direction curDirection = direction;

	pair<int, int> headPosition = snakeSegments.front()->getElementPtr()->GetPos();
	FloatRect virtualCollider = FloatRect({ (float)headPosition.first, (float)headPosition.second }, { GRIDSIZE, GRIDSIZE });

	bool warning = false;

	#pragma region Look for obstacles



	for (size_t i = 0; i < kMaxLookAheadDistance; i++)	//check i spaces ahead
	{

		switch (curDirection)
		{
		case N:
			virtualCollider = FloatRect({ (float)headPosition.first, (float)headPosition.second - (GRIDSIZE * (i + 1)) }, { GRIDSIZE, GRIDSIZE });
			break;
		case E:
			virtualCollider = FloatRect({ (float)headPosition.first + (GRIDSIZE * (i + 1)) , (float)headPosition.second }, { GRIDSIZE, GRIDSIZE });
			break;
		case S:
			virtualCollider = FloatRect({ (float)headPosition.first, (float)headPosition.second + (GRIDSIZE * (i + 1)) }, { GRIDSIZE, GRIDSIZE });
			break;
		case W:
			virtualCollider = FloatRect({ (float)headPosition.first - (GRIDSIZE * (i + 1)) , (float)headPosition.second }, { GRIDSIZE, GRIDSIZE });
			break;
		default:
			virtualCollider = FloatRect({ (float)headPosition.first, (float)headPosition.second + (GRIDSIZE * (i + 1)) }, { GRIDSIZE, GRIDSIZE });
			break;
		}




		//Check for collision with other snakes
		for (Snake* snake : otherSnakes) {
			if (snake->getIsAlive()) {
				if (SimulateCollision(snake, virtualCollider)) {
					cout << "AI sensed SNAKE collision ahead" << endl;
					warning = true;
					goto endloop;
					//exit;
				}
			}
		}

		//Check for wall collision
		Vector2i rectPos = rectCenter(virtualCollider);
		//cout << "RECT POS Y IS " << rectPos.x << ", COMPARED TO SCREEN HEIGHT " << SCREEN_W << endl;
		//cout << "RECT POS X IS " << rectPos.y << ", COMPARED TO SCREEN HEIGHT " << SCREEN_H << endl;

		if (superRespiration && (curOxygen >= kOxLevelBeforeAttemptingToSurface)) {	//if vicious AI, never attempt to surface unless oxygen is low
			if (rectPos.x < 0 || rectPos.x >= SCREEN_W || rectPos.y >= SCREEN_H || rectPos.y < waterSurfacePos) {
				cout << "AI sensed WALL collision ahead (OR WATER)" << endl;
				warning = true;
				goto endloop;
			}
		}
		else {
			if (rectPos.x < 0 || rectPos.x >= SCREEN_W || rectPos.y >= SCREEN_H || rectPos.y < 0) {
				cout << "AI sensed WALL collision ahead" << endl;
				warning = true;
				goto endloop;
			}
		}


	}

endloop:

#pragma endregion

	#pragma region Handle obstacle is warned


	if (warning) {

		cout << "HAS BEEN WARNED" << endl;


		if (direction == Direction::E) {

			prevHorizontalDirection = Direction::E;
		}
		else if (direction == Direction::W) {
			prevHorizontalDirection = Direction::W;

		}

		Direction newDirection = Direction::N;

		//Decide to randomly turn left or right
		bool dirChoice = RANDBOOL;
		switch (curDirection)
		{
		case N:

			if (dirChoice) {
				newDirection = Direction::W;
			}
			else {
				newDirection = Direction::E;
			}

			break;
		case E:

			if (dirChoice) {
				newDirection = Direction::N;
			}
			else {
				newDirection = Direction::S;
			}

			break;
		case S:

			if (dirChoice) {
				newDirection = Direction::E;
			}
			else {
				newDirection = Direction::W;
			}

			break;
		case W:

			if (dirChoice) {
				newDirection = Direction::S;
			}
			else {
				newDirection = Direction::N;
			}

			break;
		default:

			if (dirChoice) {
				newDirection = Direction::W;
			}
			else {
				newDirection = Direction::E;
			}

			break;
		}

		//Set new direction and exit AI process

		//Disallows snake from moving North when out of water
		if (!isSubmerged) {
			if (direction == Direction::N) {
				if (RANDBOOL) {
					direction = Direction::E;
				}
				else {
					direction = Direction::W;
				}

			}
		}

		//Check if new direction is obstructed
		NewDirGuard(virtualCollider, newDirection, headPosition);

		direction = newDirection;

		return;

	}

#pragma endregion


	//If at this stage, new secodnary goal is to make sure it is not drowning

	#pragma region Handle recovering oxygen

	if (curOxygen < kOxLevelBeforeAttemptingToSurface) {

		cout << "OXYGEN LOW. ATTEMPTING TO SURFACE" << endl;

		Direction newDirection;

		switch (curDirection)
		{
		case N:
			//No need to change direction
			break;
		case E:
			newDirection = Direction::N;
			break;
		case S:
			if (RANDBOOL) {
				newDirection = Direction::E;
			}
			else {
				newDirection = Direction::W;
			}
			break;
		case W:
			newDirection = Direction::N;

			break;
		default:
			break;
		}


		//Guard
		NewDirGuard(virtualCollider, newDirection, headPosition);

		direction = newDirection;

		return;

	}

	#pragma endregion


	//Prevent any obstructing food detection actions from messing with the snake when out of water
	if (!isSubmerged) {
		return;
	}

	//If at this stage, new tertiary goal is to search for food

	#pragma region Handle food

	if (!hasFoodTarget)
	{


		//Find food with the closest manhattan distance

		Direction newDirection = direction;

		Food* targetFood = nullptr;

		int closestFoodDist = INT_MAX;


		for (auto fd : foodManager->getAllFood()) {

			//Vector2i manhat = manhattanDistVec(getHeadVec(), fd.GetPosVec());

			bool inLineWithFood_vert = false;
			bool inLineWithFood_horiz = false;

			int manhatX;
			int manhatY;

			if (fd.GetPosVec().x == headPosition.first) {	//in same column as food

				cout << "FOUND FOOD IN COLUMN" << endl;

				/*hasFoodTarget = true;
				mainTargetFood = &fd;*/


				inLineWithFood_vert = true;


				targetFood = &fd;

				mainTargetFood = &fd;

				Vector2i targetPosition = targetFood->GetPosVec();
				Vector2i thisPos = getHeadVec();


				if (thisPos.y < targetPosition.y) {	//food is more south

					switch (curDirection) {
					case N:
						//DO NOTHING IF FACING AWAY FROM FOOD
						break;
					case E:
						newDirection = Direction::S;
						break;
					case S:
						//no need to change direction
						break;
					case W:
						newDirection = Direction::S;

						break;
					default:
						break;
					}


				}
				else {								//food is more north
					switch (curDirection) {
					case N:
						//no need to change direction

						break;
					case E:
						newDirection = Direction::N;
						break;
					case S:


						//DO NOTHING IF FACING AWAY FROM FOOD

						break;
					case W:
						newDirection = Direction::N;

						break;
					default:
						break;
					}



				}

				//GUARD

				NewDirGuard(virtualCollider, newDirection, headPosition);

				direction = newDirection;
				return;




				//goto foundFood;
			}
			else if (fd.GetPosVec().y == headPosition.second) { //same row as food

				cout << "FOUND FOOD IN ROW" << endl;
				/*hasFoodTarget = true;
				mainTargetFood = &fd;*/

				targetFood = &fd;
				mainTargetFood = &fd;


				Vector2i targetPosition = targetFood->GetPosVec();
				Vector2i thisPos = getHeadVec();


				if (thisPos.x < targetPosition.x) {	//food is more right

					switch (curDirection) {
					case N:
						newDirection = Direction::E;
						break;
					case E:

						//no need to change direction

						break;
					case S:
						newDirection = Direction::E;

						break;
					case W:

						//DO NOTHING IF FACING AWAY FROM FOOD
						break;
					default:
						break;
					}


				}
				else {								//food is more left
					switch (curDirection) {
					case N:
						newDirection = Direction::W;
						break;
					case E:

						//DO NOTHING IF FACING AWAY FROM FOOD



						break;
					case S:
						newDirection = Direction::W;

						break;
					case W:

						//no need to change direction


						break;
					default:
						break;
					}



				}

				//GUARD

				NewDirGuard(virtualCollider, newDirection, headPosition);

				direction = newDirection;
				return;



			}

		}

	}
	else {

		if (mainTargetFood == nullptr) {
			cout << "AI error?" << endl;
			hasFoodTarget = false;
			cout << "NOW SEARCHING FOR NEW FOOD" << endl;
			return;
		}



		//don't care anymore. it's smart enough.

	}

	#pragma endregion

	//OLD ATTEMPT AT MORE IN DEPTH SEARCH
	#pragma region JUNK



	/*
	Food* targetFood = nullptr;

	int closestFoodDist = INT_MAX;


	for (auto fd : foodManager->getAllFood()) {

		int dist = manhattanDist(getHeadVec(), fd.GetPosVec());

		if (dist < closestFoodDist) {
			closestFoodDist = dist;
			targetFood = &fd;

		}

	}

	if (targetFood == nullptr) {
		cout << "WARNING: AI HAS NOT DETECTED A TARGET." << endl;
		return;
	}

	Vector2i targetPosition = targetFood->GetPosVec();
	Vector2i thisPos = getHeadVec();

	int distX = abs(targetPosition.x - thisPos.x);
	int distY = abs(targetPosition.y - thisPos.y);


	Direction newDirection = Direction::N;	//init

	if (distX < distY && distX != 0) {	//Closer horizontal

		if (thisPos.x < targetPosition.x) {	//food is to snake's right
			switch (curDirection)
			{
			case N:
				newDirection = Direction::E;
				break;
			case E:
				//Do nothing
				break;
			case S:
				newDirection = Direction::E;
				break;
			case W:

				if (thisPos.y < targetPosition.y) {	//food is lower down
					newDirection = Direction::S;
				}
				else{
					newDirection = Direction::N;
				}
				break;
			default:
				break;
			}
		}
		else {	//food is to snake's left
			switch (curDirection)
			{
			case N:
				newDirection = Direction::W;
				break;
			case E:
				if (thisPos.y < targetPosition.y) {	//food is lower down
					newDirection = Direction::S;
				}
				else {
					newDirection = Direction::N;
				}
				break;
			case S:
				newDirection = Direction::W;
				break;
			case W:
				//Do nothing
				break;
			default:
				break;
			}
		}

	}
	else if (distY < distX && distY != 0) {	//closer vertical

		if (thisPos.y < targetPosition.y) {	//food is to snake's south
			switch (curDirection)
			{
			case N:
				if (thisPos.x < targetPosition.x) {	//food is more to the right
					newDirection = Direction::E;
				}
				else {
					newDirection = Direction::W;
				}
				break;
			case E:
				newDirection = Direction::S;
				break;
			case S:
				//do nothing
				break;
			case W:
				newDirection = Direction::S;

				break;
			default:
				break;
			}
		}
		else {								//food is to snake's north
			switch (curDirection)
			{
			case N:
				//do nothing
				break;
			case E:
				newDirection = Direction::N;
				break;
			case S:
				//do nothing
				if (thisPos.x < targetPosition.x) {	//food is more to the right
					newDirection = Direction::E;
				}
				else {
					newDirection = Direction::W;
				}
				break;
			case W:
				newDirection = Direction::N;

				break;
			default:
				break;
			}
		}

	}
	else if (distY == 0){	//on the same row




		switch (curDirection)
		{
		case N:

			if (thisPos.x < targetPosition.x) {	//food is to the right
				newDirection = Direction::E;
			}
			else {
				newDirection = Direction::W;
			}

			break;
		case E:

			if (thisPos.x < targetPosition.x) {	//food is to the right
				//Do nothing
			}
			else {
				newDirection = Direction::N;
			}

			break;
		case S:
			//do nothing
			if (thisPos.x < targetPosition.x) {	//food is to the right
				newDirection = Direction::E;
			}
			else {
				newDirection = Direction::W;
			}
			break;
		case W:
			if (thisPos.x < targetPosition.x) {	//food is to the right
				//Do nothing

			}
			else {
				newDirection = Direction::S;
			}

			break;
		default:
			break;
		}

	}
	else if (distX == 0) {	//same column

		switch (curDirection)
		{
		case N:

			if (thisPos.y < targetPosition.y) {	//food is to the south
				newDirection = Direction::E;
			}
			else {
				//do nothing
			}

			break;
		case E:

			if (thisPos.y < targetPosition.y) {	//food is to the south
				newDirection = Direction::S;
			}
			else {
				newDirection = Direction::N;
			}

			break;
		case S:
			if (thisPos.y < targetPosition.y) {	//food is to the south
				//do nothing
			}
			else {
				newDirection = Direction::W;
			}
			break;
		case W:
			if (thisPos.y < targetPosition.y) {	//food is to the south
				newDirection = Direction::S;
			}
			else {
				newDirection = Direction::N;
			}

			break;
		default:
			break;
		}
	}

	NewDirGuard(virtualCollider, newDirection, headPosition);


	direction = newDirection;

	*/
#pragma endregion

}

void AISnake::NewDirGuard(FloatRect& virtualCollider, Direction& newDirection, pair<int, int>& headPosition) {

	bool flipDirection = false;
	switch (newDirection)
	{
	case N:
		virtualCollider = FloatRect({ (float)headPosition.first, (float)headPosition.second - (GRIDSIZE) }, { GRIDSIZE, GRIDSIZE });
		break;
	case E:
		virtualCollider = FloatRect({ (float)headPosition.first + (GRIDSIZE) , (float)headPosition.second }, { GRIDSIZE, GRIDSIZE });
		break;
	case S:
		virtualCollider = FloatRect({ (float)headPosition.first, (float)headPosition.second + (GRIDSIZE) }, { GRIDSIZE, GRIDSIZE });
		break;
	case W:
		virtualCollider = FloatRect({ (float)headPosition.first - (GRIDSIZE) , (float)headPosition.second }, { GRIDSIZE, GRIDSIZE });
		break;
	default:
		virtualCollider = FloatRect({ (float)headPosition.first, (float)headPosition.second + (GRIDSIZE) }, { GRIDSIZE, GRIDSIZE });
		break;
	}

	//Check for collision with other snakes
	for (Snake* snake : otherSnakes) {
		if (snake->getIsAlive()) {
			if (SimulateCollision(snake, virtualCollider)) {
				cout << "AI sensed collision at new position (SNAKE). Attempting alternate route." << endl;
				flipDirection = true;
			}
		}
	}
	//Check for wall collision
	Vector2i rectPos = rectCenter(virtualCollider);
	if (rectPos.x < 0 || rectPos.x >= SCREEN_W || rectPos.y >= SCREEN_H || rectPos.y < 0) {
		cout << "AI sensed collision at new position (WALL). Attempting alternate route." << endl;
		flipDirection = true;
	}

	if (flipDirection) {
		switch (newDirection)
		{
		case N:
			newDirection = Direction::S;
			break;
		case E:
			newDirection = Direction::W;

			break;
		case S:
			newDirection = Direction::N;

			break;
		case W:
			newDirection = Direction::E;
			break;
		default:
			newDirection = Direction::S;
			break;
		}
	}
}