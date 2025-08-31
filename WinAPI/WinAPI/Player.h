#pragma once

#include "GameState.h"
#include "PlayerState.h"
#include "AStar.h"

class Player
{
public:
	void move(bool wasd[4]);
	void loadingBullets(bool arrow[4]);

private:
	bool okToGo(POINT pos);

private:
	AStar aStar;
};

