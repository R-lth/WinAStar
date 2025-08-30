#include <random>
#include "Monster.h"


void Monster::spawnMonster(const vector<vector<int>>& grid)
{
    int i = rand() % spawnPos.size();

    pos = spawnPos[i];

    if (collistion.okToGo(pos, grid) && !collistion.checkMonsterCollision(pos, monsterPos))
    {
        // TODO. game 객체의 역할 
        //////////////////////////////////////////////
        deque<POINT> path = aStar.findPath(monster, player, playGrid);
        pathInfo.emplace_back(path);
        int id = pathInfo.size() - 1;
        //////////////////////////////////////////////
        monsterPos.insert({ id, monster });
    }
}

void Monster::moveMonster(POINT player, const vector<vector<int>>& grid)
{
    for (int id = 0; id < monsterPos.size(); ++id)
    {
        POINT next = monsterPos[id];

        // TODO. game 객체의 역할?
        while (!pathInfo[id].empty() && monsterPos[id].x == next.x && monsterPos[id].y == next.y)
        {
            next = pathInfo[id].front();
            pathInfo[id].pop_front();
        }

        if (!collistion.okToGo(next, grid) || collistion.checkMonsterCollision(id, next, monsterPos))
        {
            continue;
        }

        // 키가 눌리지 않아도, 플레이어와 몬스터 간의 충돌이 가능하도록
        if (monsterPos[id].x != player.x  && monsterPos[id].y != player.y)
        {
            // TODO. game 객체의 역할?
            // TODO. A*의 대각선 {x, y} 값 고려하기
            monsterPos[id] = next;
            deque<POINT> path = aStar.findPath(monsterPos[id], player, playGrid);
            pathInfo[id] = path;
        }
    }
}
