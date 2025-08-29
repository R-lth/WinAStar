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

// TODO. ���� �����ڸ� private�� �����ϱ�
// init()�� update() �׸��� render()�� �ű��
public:
    vector<vector<int>> gridGround() 
    {
        vector<vector<int>> grid(20, vector<int>(20, 0));

        // �𼭸��� ���
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

    // TODO. ���� �÷��̾��ϰ� ���Ϳ� �ű��
    bool mFilp = false;
};