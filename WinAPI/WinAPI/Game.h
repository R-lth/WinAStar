#pragma once

#include <vector>
#include <list>
#include <random>
#include "Windows.h"
#include "Resource.h"

#include "Setting.h"
#include "GameState.h"
#include "Player.h"

// TODO. 헤더 인클루드 정리

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
	void init(HWND hWnd);
	void update(HWND hWnd, WPARAM wParam);
	void render(HDC hdc, HINSTANCE hInst);

    void renderBegin(HDC hdc, HINSTANCE hInst);
    void renderPlay();
    void renderEnd();

private:
    void setPlayGround();

    // TODO. 접근 지정자 수정
public:
    const vector<pair<POINT, float>> direction =
    {
        { {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
        { {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
    };

private:
    GameState gameState;
    Player player;

private:
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