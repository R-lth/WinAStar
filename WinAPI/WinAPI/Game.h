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

// TODO. 접근 지정자를 private로 변경하기
// init()과 update() 그리고 render()에 옮기기
public:
    vector<vector<int>> gridGround() 
    {
        vector<vector<int>> grid(20, vector<int>(20, 0));

        // 모서리와 통로
        for (int i = 0; i < 20; ++i)
        {
            int v = (i >= 7 && i < 13) ? 0 : 1;
            grid[0][i] = v;
            grid[19][i] = v;
            grid[i][0] = v;
            grid[i][19] = v;
        }

        return grid;
    }
    list<pair<int, POINT>> setGun(int dir) 
    {
        return list<pair<int, POINT>>(dir);
    };

    bool CollideWithPlayer(POINT pos)
    {
        return (pos.x == playerBmp.x && pos.y == playerBmp.y);
    }

    bool CollideWithOtherMonsters(int id, POINT pos)
    {
        for (const pair<int, POINT>& it : monsterPos)
        {
            if (id == it.first)
            {
                continue;
            }

            POINT other = it.second;
            if (pos.x == other.x && pos.y == other.y)
            {
                return true;
            }
        }

        return false;
    }

// TODO. 접근 지정자 수정
public:
    RECT rect;
    int width;
    int height;
    const int cell = 20;
    const int n = 20;

    POINT playerPos;
    AStar aStar;

    const vector<pair<POINT, float>> direction =
    {
        { {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
        { {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
    };

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
    
    vector<vector<int>> grid;
    // 
    vector<deque<POINT>> pathInfo;

    bool gameOver = false;
    bool waiting = false;

    HDC back, scr;
    HBITMAP bmp, originalBmp;

    HBITMAP groundBmp[2]; // Aisle, Brick
    HBITMAP blackBmp;
    HBITMAP uiTextBmp[7]; // Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7; 

    HBITMAP playerBmp[4]; // Goal, GoalLeft, GoalRight, GoalUp
    HBITMAP bulletBmp;
    HBITMAP monsterBmp[2]; // Character1, Character2
    HBITMAP deadBmp;

    // TODO. 각각 플레이어하고 몬스터에 옮기기
    bool mFilp = false;
};