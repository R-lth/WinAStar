#pragma once

#include <vector>
#include <deque>
#include "GameState.h"
#include "Collision.h"
#include "AStar.h"

using namespace std;

class Monster
{
public:
	Monster() = default;
	~Monster() = default;

public:
    void setMFilp();
    bool getMFlip();
	void spawnMonster(Position player);
	void moveMonster(Position player);

private:
    const vector<Position> spawnPos =
    {
        {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12},
        {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12},
        {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0},
        {7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}
    };

    Position pos;
    bool mFilp = false;
    GameState gameState;
    Collision collision;
    AStar aStar;
};