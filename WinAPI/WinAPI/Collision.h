#pragma once
#include "windef.h"
#include <vector>
#include <map>
#include <list>

using namespace std;

class Collision
{
    // TODO. grid�� monsterPos�� �Ź� �Ű������� �޴� �� ��� �ٸ� ����� �����غ���
public:
    bool isInRange(const POINT pos, const int n);

    bool isObstacle(const POINT pos, const vector<vector<int>>& grid);

    bool okToGo(const POINT pos, const vector<vector<int>>& grid);

    bool checkPlayerMonsterCollision(const POINT pos, map<int, POINT>& monsterPos);

    bool checkMonsterCollision(const POINT pos, const map<int, POINT>& monsterPos);

    bool checkMonsterCollision(int id, const POINT pos, const map<int, POINT>& monsterPos);
};