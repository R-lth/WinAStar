#include <random>
#include "Monster.h"


void Monster::spawnMonster()
{
    int i = rand() % spawnPos.size();

    pos = spawnPos[i];

    if (collision.okToGo(pos) && !collision.checkMonsterCollision(pos))
    {
        // TODO. game 객체의 역할 
        //////////////////////////////////////////////
        deque<POINT> path = aStar.findPath(monster, player, playGrid);
        pathInfo.emplace_back(path);
        int id = pathInfo.size() - 1;
        //////////////////////////////////////////////
        // TODO. 충돌 자체에서 monsterPos는 함수로 접근하는 게 맞을까
        collision.getMonsterPos().insert({id, monster});
    }
}

void Monster::moveMonster(POINT player)
{
    for (int id = 0; id < collision.getMonsterPos().size(); ++id)
    {
        POINT next = collision.getMonsterPos()[id];

        // TODO. game 객체의 역할?
        while (!pathInfo[id].empty() && collision.getMonsterPos()[id].x == next.x && collision.getMonsterPos()[id].y == next.y)
        {
            next = pathInfo[id].front();
            pathInfo[id].pop_front();
        }

        if (!collision.okToGo(next) || collision.checkMonsterCollision(id, next))
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
