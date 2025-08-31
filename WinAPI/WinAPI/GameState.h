#pragma once

#include <vector>
#include <map>
#include <list>
#include "Windows.h"

#include "Singleton.h"

using namespace std;

class GameState : public Singleton<GameState>
{
public:
    GameState() 
    {
        grid.resize(20, vector<int>(20, 0));
        gun.resize(8);
    }

public:
    bool gameOver = false;
    bool waiting = false;
    
    // TODO.
    vector<vector<int>> grid;
    map<int, POINT> monsterPos;

    // TODO. playerInfo(싱글톤) 생성하기
    bool pHoriz = false;
    bool pUp = false;
    bool pFilp = false;
            
    POINT playerPos = {10, 10};
    list<pair<int, POINT>> gun;
};