#include "Input.h"

MoveDir Input::getMoveDir()
{
    wasd[0] = GetAsyncKeyState('W' & 0x8000);
    wasd[1] = GetAsyncKeyState('A' & 0x8000);
    wasd[2] = GetAsyncKeyState('S' & 0x8000);
    wasd[3] = GetAsyncKeyState('D' & 0x8000);

    if (wasd[0] && wasd[1]) return MoveDir::WA;
    else if (wasd[0] && wasd[3]) return MoveDir::WD;
    else if (wasd[2] && wasd[1]) return MoveDir::SA;
    else if (wasd[2] && wasd[3]) return MoveDir::SD;
    else if (wasd[0]) return MoveDir::W;
    else if (wasd[1]) return MoveDir::A;
    else if (wasd[2]) return MoveDir::S;
    else if (wasd[3]) return MoveDir::D;
}

ShootDir Input::getShootDir()
{
    arrow[0] = GetAsyncKeyState(VK_UP & 0x8000);
    arrow[1] = GetAsyncKeyState(VK_LEFT & 0x8000);
    arrow[2] = GetAsyncKeyState(VK_DOWN & 0x8000);
    arrow[3] = GetAsyncKeyState(VK_RIGHT & 0x8000);

    if (arrow[0] && arrow[1]) return ShootDir::UpLeft;
    else if (arrow[0] && arrow[3]) return ShootDir::UpRight;
    else if (arrow[2] && arrow[1]) return ShootDir::DownLeft;
    else if (arrow[2] && arrow[3]) return ShootDir::DownRight;
    else if (arrow[0]) return ShootDir::Up;
    else if (arrow[1]) return ShootDir::Left;
    else if (arrow[2]) return ShootDir::Down;
    else if (arrow[3]) return ShootDir::Right;
}
