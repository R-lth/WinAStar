#pragma once
#include "Windows.h"

#include "Singleton.h"
#include "Types.h"

class Input : public Singleton<Input>
{
public:
    MoveDir getMoveDir();
    ShootDir getShootDir();

private:
    bool wasd[4];
    bool arrow[4]; // up, left, down, right
};

