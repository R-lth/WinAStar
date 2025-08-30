#include <random>
#include "Monster.h"

void Monster::setMFilp()
{
    mFilp = !mFilp;
}

bool Monster::getMFlip()
{
    return mFilp;
}

void Monster::spawnMonster(Position player)
{
    int i = rand() % spawnPos.size();

    pos = spawnPos[i];

    if (collision.okToGo(pos) && !collision.checkMonsterCollision(pos))
    {
        // TODO. game 객체의 역할 
        //////////////////////////////////////////////
        deque<Position> path = aStar.findPath(pos, player);
        gameState.pathInfo.emplace_back(path);
        int id = gameState.pathInfo.size() - 1;
        //////////////////////////////////////////////
        gameState.monsterPos.insert({id, pos});
    }
}

void Monster::moveMonster(Position player)
{
    for (int id = 0; id < gameState.monsterPos.size(); ++id)
    {
        Position next = gameState.monsterPos[id];

        while (!gameState.pathInfo[id].empty() && gameState.monsterPos[id].x == next.x && gameState.monsterPos[id].y == next.y)
        {
            next = gameState.pathInfo[id].front();
            gameState.pathInfo[id].pop_front();
        }

        if (!collision.okToGo(next) || collision.checkMonsterCollision(next, id))
        {
            continue;
        }

        gameState.monsterPos[id] = next;
        deque<Position> path = aStar.findPath(gameState.monsterPos[id], player);
        gameState.pathInfo[id] = path;
    }
}
