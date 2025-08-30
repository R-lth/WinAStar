#pragma once

#include <list>
#include "Types.h"
#include "GameState.h"
#include "Gun.h"

using namespace std;

class Collision
{
public:
    bool isInRange(const Position pos);

    bool isObstacle(const Position pos);

    bool okToGo(const Position pos);

    bool checkPlayerMonsterCollision(const Position pos);

    bool checkMonsterCollision(const Position pos);

    bool checkMonsterCollision(const Position pos, int id);

    bool shot(const Position pos, list<pair<ShootDir, Position>>::iterator& it);

private:
    GameState gameState;
    Gun gun;
};