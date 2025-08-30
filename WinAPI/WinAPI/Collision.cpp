#include "Collision.h"

bool Collision::isInRange(const POINT pos)
{
    const int n = gameState.grid.size();
    return (pos.x >= 0 && pos.x < n && pos.y >= 0 && pos.y < n);
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
        if (pos.x == monster.x && pos.y && monster.y)
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

bool Collision::checkMonsterCollision(int id, const POINT pos)
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

map<int, POINT>& Collision::getMonsterPos()
{
    return gameState.monsterPos;
}
