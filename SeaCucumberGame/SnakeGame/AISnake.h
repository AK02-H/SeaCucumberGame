#pragma once
#include "Snake.h"

class AISnake : public Snake
{
private:

	//'Intelligence'
	const int kMaxLookAheadDistance = 3;
	const int kOxLevelBeforeAttemptingToSurface = 40;

	bool hasFoodTarget = false;
	Food* mainTargetFood;

public:

	//Keeps parent constructor
	using Snake::Snake;

	void HandleInput();

	bool SimulateCollision(Collider* collider, FloatRect virtuaRect) {
		vector<FloatRect> rectsToCheckAgainst = collider->CollisionPoints();

		for (FloatRect rect : rectsToCheckAgainst) {
			if (AABB(virtuaRect, rect)) return true;
		}

		return false;
	}

	int manhattanDist(Vector2i start, Vector2i target) {

		int x = abs(target.x - start.x);
		int y = abs(target.y - start.y);

		return x + y;
	}

	Vector2i manhattanDistVec(Vector2i start, Vector2i target) {

		int x = abs(target.x - start.x);
		int y = abs(target.y - start.y);

		return { x, y };
	}

	Vector2i rectCenter(FloatRect rect) {
		int x = rect.left + rect.width / 2;
		int y = rect.top + rect.height / 2;

		return { x, y };
	}

	//Prevents snake from moving into obstacles
	void NewDirGuard(FloatRect& virtualCollider, Direction& newDirection, pair<int, int>& headPosition);	
};

