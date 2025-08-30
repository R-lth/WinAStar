#include "Player.h"
#include "Input.h"


void Player::setPos(Position pos)
{
    this->pos = pos;
}

Position Player::getPos()
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

Position Player::movePlayer(HWND hWnd)
{
    MoveDir dir = Input::Get().getMoveDir();

    Position next = pos;
    pHorizontal = false;

    switch (dir)
    {
    case MoveDir::WA:
        next.x -= 1;
        next.y -= 1;
        pVertical = true;
        break;
    case MoveDir::WD:
        next.x += 1;
        next.y -= 1;
        pVertical = true;
        break;
    case MoveDir::SA:
        next.x -= 1;
        next.y += 1;
        pVertical = false;
        break;
    case MoveDir::SD:
        next.x += 1;
        next.y += 1;
        pVertical = false;
        break;
    case MoveDir::W:
        next.y -= 1;
        pVertical = true;
        break;
    case MoveDir::A:
        next.x -= 1;
        pHorizontal = true;
        pFilp = !pFilp;
        break;
    case MoveDir::S:
        next.y += 1;
        pVertical = false;
        break;
    case MoveDir::D:
        next.x += 1;
        pHorizontal = true;
        pFilp = !pFilp;
        break;
    default:
        break;
    }

    // 경계 및 장애물 검사
    if (!collision.okToGo(next))
    {
        return pos;
    }

    // 충돌 처리
    bool hit = collision.checkPlayerMonsterCollision(next);

    // 이동
    if (!hit) 
    {
        pos = next;

        // TODO. 플레이어 이동에 따른 몬스터 경로 갱신
        for (int id = 0; id < gameState.monsterPos.size(); ++id)
        {
            gameState.pathInfo[id] = aStar.findPath(gameState.monsterPos[id], pos);
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
    Position bullet = pos;

    switch (dir)
    {
    case ShootDir::UpLeft:
        bullet.x -= 1;
        bullet.y -= 1;
        break;
    case ShootDir::UpRight:
        bullet.x += 1;
        bullet.y -= 1;
        break;
    case ShootDir::DownLeft:
        bullet.x -= 1;
        bullet.y += 1;
        break;
    case ShootDir::DownRight:
        bullet.x += 1;
        bullet.y += 1;
        break;
    case ShootDir::Up:
        bullet.y -= 1;
        break;
    case ShootDir::Left:
        bullet.x -= 1;
        break;
    case ShootDir::Down:
        bullet.y += 1;
        break;
    case ShootDir::Right:
        bullet.x += 1;
        break;
    default:
        break;
    }

    gun.loadingBullets({dir, bullet});
}