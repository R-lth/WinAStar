#pragma once

#include "GameState.h"

using namespace std;

class Collision
{
public:
    bool isInRange(const POINT pos);

    bool isObstacle(const POINT pos);

    bool okToGo(const POINT pos);

    bool checkPlayerMonsterCollision(const POINT pos);

    bool checkMonsterCollision(const POINT pos);

    bool checkMonsterCollision(int id, const POINT pos);

    map<int, POINT>& getMonsterPos();

private:
    GameState gameState;
};