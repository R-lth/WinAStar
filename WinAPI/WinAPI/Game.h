#pragma once

#include <vector>
#include <list>
#include "AStar.h"
#include "Resource.h"

// TODO. ��� ��Ŭ��� ����
// ��ǻ� resource.h�� winAPI.h�� ���Ե� ����

using namespace std;

// TODO. ���� ����
//class AStar;

// TODO. ������ ����

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

// TODO. �Լ�. ���� �����ڸ� private�� �����ϱ�
// init()�� update() �׸��� render()�� �ű��
public:
    void setPathInfo();
    vector<deque<POINT>> getPathInfo();

// TODO. ���� ������ ����
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
        // index�� char �Ǵ�
        // ���ʿ���
        // ����
        // ����
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
};