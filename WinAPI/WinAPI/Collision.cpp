#include "Collision.h"

bool Collision::isInRange(const POINT pos, const int n)
{
    return (pos.x >= 0 && pos.x < n && pos.y >= 0 && pos.y < n);
}

bool Collision::isObstacle(const POINT pos, const vector<vector<int>>& grid)
{
    return (grid[pos.y][pos.x]);
}

bool Collision::okToGo(const POINT pos, const vector<vector<int>>& grid)
{
    const int n = grid.size();
    return (isInRange(pos, n) && !isObstacle(pos, grid));
}

bool Collision::checkPlayerMonsterCollision(const POINT pos, map<int, POINT>& monsterPos)
{
    for (const pair<int, POINT>& it : monsterPos)
    {
        POINT monster = it.second;
        if (pos.x == monster.x && pos.y && monster.y)
        {
            monsterPos.erase(it.first);
            return true;
        }
    }

    return false;
}

bool Collision::checkMonsterCollision(const POINT pos, const map<int, POINT>& monsterPos)
{
    for (int i = 0; i < monsterPos.size(); ++i)
    {

        if (pos.x == monsterPos[i].x && pos.y == monsterPos[i].y)
        {
            return true;
        }
    }

    return false;
}


bool Collision::checkMonsterCollision(int id, const POINT pos, const map<int, POINT>& monsterPos)
{
    for (int i = 0; i < monsterPos.size(); ++i) 
    {
        if (id == i) 
        {
            continue;
        }

        if (pos.x == monsterPos[i].x && pos.y == monsterPos[i].y) 
        {
            return true;
        }
    }

    return false;
}
