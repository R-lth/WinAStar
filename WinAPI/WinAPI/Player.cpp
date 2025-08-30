#include "Player.h"
#include "Input.h"


void Player::setPos(POINT pos)
{
    this->pos = pos;
}

POINT Player::getPos()
{
    return pos;
}

bool Player::getVertical()
{
    return pVertical;
}

bool Player::getHorizontal()
{
    return pHorizontal;
}

bool Player::getFilp()
{
    return pFilp;
}

POINT Player::movePlayer(HWND hWnd)
{
    MoveDir dir = Input::Get().getMoveDir();

    POINT next = pos;
    pHorizontal = false;

    if (dir == MoveDir::WA)
    {
        next.x -= 1;
        next.y -= 1;
        pVertical = true;
    }
    else if (dir == MoveDir::WD)
    {
        next.x += 1;
        next.y -= 1;
        pVertical = true;
    }
    else if (dir == MoveDir::SA)
    {
        next.x -= 1;
        next.y += 1;
        pVertical = false;
    }
    else if (dir == MoveDir::SD)
    {
        next.x += 1;
        next.y += 1;
        pVertical = false;
    }
    else if (dir == MoveDir::W)
    {
        next.y -= 1;
        pVertical = true;
    }
    else if (dir == MoveDir::A)
    {
        next.x -= 1;
        pHorizontal = true;
        pFilp = !pFilp;
    }
    else if (dir == MoveDir::S)
    {
        next.y += 1;
        pVertical = false;
    }
    else if (dir == MoveDir::D)
    {
        next.x += 1;
        pHorizontal = true;
        pFilp = !pFilp;
    }

    // 경계 및 장애물 검사
    if (!collision.okToGo(pos))
    {
        return;
    }

    // 충돌 처리
    bool hit = collision.checkPlayerMonsterCollision(pos);

    // 이동
    if (!hit) 
    {
        pos = next;

        // TODO. 플레이어 이동에 따른 몬스터 경로 갱신
        // 이건 게임 객체의 역할이 아닐까
        for (int id = 0; id < gameState.monsterPos.size(); ++id)
        {
            gameState.pathInfo[id] = aStar.findPath(gameState.monsterPos[id], pos, gameState.grid);
        }
    }
    // 게임 종료
    else 
    {
        gameState.waiting = true;
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
        KillTimer(hWnd, 3);

        // 2초 후 
        SetTimer(hWnd, 4, 2000, NULL);
    }

    return pos;
}

void Player::shoot()
{
    ShootDir dir = Input::Get().getShootDir();
    POINT bullet = pos;

    if (dir == ShootDir::UpLeft)
    {
        bullet.x -= 1;
        bullet.y -= 1;
    }
    else if (dir == ShootDir::UpRight)
    {
        bullet.x += 1;
        bullet.y -= 1;
    }
    else if (dir == ShootDir::DownLeft)
    {
        bullet.x -= 1;
        bullet.y += 1;
    }
    else if (dir == ShootDir::DownRight)
    {
        bullet.x += 1;
        bullet.y += 1;
    }
    else if (dir == ShootDir::Up)
    {
        bullet.y -= 1;
    }
    else if (dir == ShootDir::Left)
    {
        bullet.x -= 1;
    }
    else if (dir == ShootDir::Down)
    {
        bullet.y += 1;
    }
    else if (dir == ShootDir::Right)
    {
        bullet.x += 1;
    }
    
    gun.loadingBullets({dir, pos});
    gun.shootBullets();
}