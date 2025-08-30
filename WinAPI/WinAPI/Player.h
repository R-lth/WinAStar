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
#include "GameState.h"

using namespace std;

class Player
{
public:
	Player() = default;
	~Player() = default;
	
public:
    void setPos(POINT pos);
    POINT getPos();
    bool getVertical();
    bool getHorizontal();
    bool getFilp();

    // TODO. grid하고 monsterPos는 전역으로 처리하는 게 나은지
    // 아니면 Game에서 책임지는 게 나은지
	POINT movePlayer(HWND hWnd);
    void shoot();

private:
	POINT pos;
    bool pVertical = false;  // 수직(상하)
    bool pHorizontal = false;     // 수평(좌우)
    bool pFilp = false;
    
    Collision collision;
    Gun gun;
    GameState gameState;
};

