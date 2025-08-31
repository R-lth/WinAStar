#pragma once

#include <vector>
#include <map>
#include "Windows.h"

#include "Singleton.h"

using namespace std;

class GameState : public Singleton<GameState>
{
public:
    GameState() 
    {
        grid.resize(20, vector<int>(20, 0));
    }

    // TODO. setter·getter
public:
    bool gameOver = false;
    bool waiting = false;
    
    // TODO.
    vector<vector<int>> grid;
    map<int, POINT> monsterPos;
};