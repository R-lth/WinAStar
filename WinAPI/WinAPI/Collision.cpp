#include "Collision.h"

bool Collision::isInRange(const Position pos)
{
    return (pos.x >= 0 && pos.x < gameState.n && pos.y >= 0 && pos.y < gameState.n);
}

bool Collision::isObstacle(const Position pos)
{
    return (gameState.grid[pos.y][pos.x]);
}

bool Collision::okToGo(const Position pos)
{
    return (isInRange(pos) && !isObstacle(pos));
}

bool Collision::checkPlayerMonsterCollision(const Position pos)
{
    for (const pair<int, Position>& it : gameState.monsterPos)
    {
        Position monster = it.second;
        if (pos.x == monster.x && pos.y == monster.y)
        {
            gameState.monsterPos.erase(it.first);
            return true;
        }
    }

    return false;
}

bool Collision::checkMonsterCollision(const Position pos)
{
    for (int i = 0; i < gameState.monsterPos.size(); ++i)
    {

        if (pos.x == gameState.monsterPos[i].x && pos.y == gameState.monsterPos[i].y)
        {
            return true;
        }
    }

    return false;
}

bool Collision::checkMonsterCollision(const Position pos, int id)
{
    for (int i = 0; i < gameState.monsterPos.size(); ++i)
    {
        if (id == i) 
        {
            continue;
        }

        if (pos.x == gameState.monsterPos[i].x && pos.y == gameState.monsterPos[i].y)
        {
            return true;
        }
    }

    return false;
}

bool Collision::shot(const Position pos, list<pair<ShootDir, Position>>::iterator& it)
{
    for (int id = 0; id < gameState.monsterPos.size(); ++id)
    {
        if (pos.x == gameState.monsterPos[id].x && pos.y == gameState.monsterPos[id].y)
        {
            //
            it = gun.getGun().erase(it);
            //
            gameState.monsterPos.erase(id);
            return true;
        }
    }

    return false;
}