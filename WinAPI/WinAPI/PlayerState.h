#pragma once

#include <vector>
#include <list>
#include "Windows.h"

#include "Singleton.h"

using namespace std;

// TODO. GameState 하위로 관리할지에 대해서 고민해 보기
class PlayerState : public Singleton<PlayerState> 
{
public:
    PlayerState()
    {
        gun.resize(8);
    }

    // TODO. setter·getter
public:
    POINT playerPos = { 10, 10 };

    bool pHoriz = false;
    bool pVert = false;
    bool pFilp = false;
    
    list<pair<int, POINT>> gun;
};