#include "Collision.h"

bool Collision::isInRange(const POINT pos)
{
    return (pos.x >= 0 && pos.x < gameState.n && pos.y >= 0 && pos.y < gameState.n);
}

bool Collision::isObstacle(const POINT pos)
{
    return (gameState.grid[pos.y][pos.x]);
}

bool Collision::okToGo(const POINT pos)
{
    return (isInRange(pos) && !isObstacle(pos));
}

bool Collision::checkPlayerMonsterCollision(const POINT pos)
{
    for (const pair<int, POINT>& it : gameState.monsterPos)
    {
        POINT monster = it.second;
        if (pos.x == monster.x && pos.y == monster.y)
        {
            gameState.monsterPos.erase(it.first);
            return true;
        }
    }

    return false;
}

bool Collision::checkMonsterCollision(const POINT pos)
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

bool Collision::checkMonsterCollision(const POINT pos, int id)
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

bool Collision::shot(const POINT pos, list<pair<ShootDir, POINT>>::iterator& it)
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