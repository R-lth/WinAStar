#pragma once
#include "windef.h"
#include <vector>
#include <map>
#include <list>

using namespace std;

class Collision
{
    // TODO. grid와 monsterPos를 매번 매개변수로 받는 것 대신 다른 방법을 생각해보기
public:
    bool isInRange(const POINT pos, const int n);

    bool isObstacle(const POINT pos, const vector<vector<int>>& grid);

    bool okToGo(const POINT pos, const vector<vector<int>>& grid);

    bool checkPlayerMonsterCollision(const POINT pos, map<int, POINT>& monsterPos);

    bool checkMonsterCollision(const POINT pos, const map<int, POINT>& monsterPos);

    bool checkMonsterCollision(int id, const POINT pos, const map<int, POINT>& monsterPos);
};