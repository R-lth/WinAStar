#pragma once

#include <vector>
#include <list>
#include "AStar.h"
#include "Resource.h"

// TODO. 헤더 인클루드 정리
// 사실상 resource.h는 winAPI.h에 포함돼 있음

using namespace std;

// TODO. 전방 선언
//class AStar;

// TODO. 프레임 제어

class Game
{
public:
	Game() = default;
	~Game() = default;

public:
	void init();
	void update();
	void render(HDC hdc, HINSTANCE hInst);

    void renderBegin(HDC hdc, HINSTANCE hInst);
    void renderPlay();
    void renderEnd();

// TODO. 함수. 접근 지정자를 private로 변경하기
// init()과 update() 그리고 render()에 옮기기
public:
    void setPathInfo();
    vector<deque<POINT>> getPathInfo();

// TODO. 접근 지정자 수정
public:
    const int n = 20;
    vector<vector<int>> grid;

    const vector<pair<POINT, float>> direction =
    {
        { {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
        { {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
    };

    //
    HDC back, scr;
    HBITMAP bmp, originalBmp;

    HBITMAP groundBmp[2]; // Aisle, Brick
    HBITMAP blackBmp;
    HBITMAP uiTextBmp[7]; // Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7; 

    HBITMAP playerBmp[4]; // Goal, GoalLeft, GoalRight, GoalUp
    HBITMAP bulletBmp;
    HBITMAP monsterBmp[2]; // Character1, Character2
    HBITMAP deadBmp;
    //
    const vector<POINT> uiSpawn
    {
        // index로 char 판단
        // 대초원의
        // 왕의
        // 모험
        {9, 6}, {10, 6}, {11, 6}, {12, 6},
        {10, 7}, {11, 7},
        {10, 8}, {11, 8}
    };
};