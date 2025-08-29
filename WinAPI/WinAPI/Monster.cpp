#include <random>
#include "Monster.h"


void Monster::spawnMonster(const vector<vector<int>>& grid)
{
    int i = rand() % spawnPos.size();

    pos = spawnPos[i];

    if (collistion.okToGo(pos, grid) && !collistion.checkMonsterCollision(pos, monsterPos))
    {
        // TODO. game ��ü�� ���� 
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

        // TODO. game ��ü�� ����?
        while (!pathInfo[id].empty() && monsterPos[id].x == next.x && monsterPos[id].y == next.y)
        {
            next = pathInfo[id].front();
            pathInfo[id].pop_front();
        }

        if (!collistion.okToGo(next, grid) || collistion.checkMonsterCollision(id, next, monsterPos))
        {
            continue;
        }

        // Ű�� ������ �ʾƵ�, �÷��̾�� ���� ���� �浹�� �����ϵ���
        if (monsterPos[id].x != player.x  && monsterPos[id].y != player.y)
        {
            // TODO. game ��ü�� ����?
            // TODO. A*�� �밢�� {x, y} �� ����ϱ�
            monsterPos[id] = next;
            deque<POINT> path = aStar.findPath(monsterPos[id], player, playGrid);
            pathInfo[id] = path;
        }
    }
}
