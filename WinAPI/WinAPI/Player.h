#pragma once

// TODO. 근데 프레임으로 제어할 건데 winproc 함수 기준으로
// 생각하는 거 아님??????????????????????

#include "windef.h"
#include <vector>
#include <map>
#include <deque>
#include <list>

#include "Types.h"
#include "Gun.h"
#include "Collision.h"

using namespace std;

class Player
{
public:
	Player() = default;
	~Player() = default;
	
public:
    // TODO. grid하고 monsterPos는 전역으로 처리하는 게 나은지
    // 아니면 Game에서 책임지는 게 나은지
	void movePlayer(const vector<vector<int>>& grid, map<int, POINT>& monsterPos);
    void shoot(const vector<vector<int>> grid, map<int, POINT>& monsterPos);

private:
	POINT pos;
    bool pHoriz = false;
    bool pFilp = false;
    bool pUp = false;
    Collision collision;
    Gun gun;
};

