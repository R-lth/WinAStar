#pragma once

#include "windef.h"
#include <vector>
#include <deque>
#include "Collision.h"

using namespace std;

class Monster
{
public:
	Monster() = default;
	~Monster() = default;

public:
	void spawnMonster(const vector<vector<int>>& grid);
	void moveMonster(POINT player, const vector<vector<int>>& grid);

private:
    const vector<POINT> spawnPos =
    {
        {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12},
        {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12},
        {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0},
        {7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}
    };

    POINT pos;
    Collision collistion;
    
    // TODO. 몬스터 좌우반전
    bool mFilp = false;
};