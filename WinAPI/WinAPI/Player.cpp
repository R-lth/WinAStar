#include "Player.h"
#include "Input.h"

void Player::movePlayer(const vector<vector<int>>& grid, map<int, POINT>& monsterPos)
{
    MoveDir dir = Input::Get().getMoveDir();

    POINT next = pos;
    pHoriz = false;

    if (dir == MoveDir::WA)
    {
        next.x -= 1;
        next.y -= 1;
        pUp = true;
    }
    else if (dir == MoveDir::WD)
    {
        next.x += 1;
        next.y -= 1;
        pUp = true;
    }
    else if (dir == MoveDir::SA)
    {
        next.x -= 1;
        next.y += 1;
        pUp = false;
    }
    else if (dir == MoveDir::SD)
    {
        next.x += 1;
        next.y += 1;
        pUp = false;
    }
    else if (dir == MoveDir::W)
    {
        next.y -= 1;
        pUp = true;
    }
    else if (dir == MoveDir::A)
    {
        next.x -= 1;
        pHoriz = true;
        pFilp = !pFilp;
    }
    else if (dir == MoveDir::S)
    {
        next.y += 1;
        pUp = false;
    }
    else if (dir == MoveDir::D)
    {
        next.x += 1;
        pHoriz = true;
        pFilp = !pFilp;
    }

    // 경계 및 장애물 검사
    if (!collision.okToGo(pos, grid))
    {
        return;
    }

    // 충돌 처리
    bool hit = collision.checkPlayerMonsterCollision(pos, monsterPos);

    // 이동
    if (!hit) 
    {
        pos = next;

        // TODO. 플레이어 이동에 따른 몬스터 경로 갱신
        // 이건 게임 객체의 역할이 아닐까
        for (int id = 0; id < monsterPos.size(); ++id)
        {
            pathInfo[id] = aStar.findPath(monsterPos[id], pos, grid);
        }
    }
    // 게임 종료
    else 
    {
        isWaiting = true;
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
        KillTimer(hWnd, 3);

        // 2초 후 
        SetTimer(hWnd, 4, 2000, NULL);
    }
}

void Player::shoot(const vector<vector<int>> grid, map<int, POINT>& monsterPos)
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
    
    gun.spawnBullet({dir, pos}, grid);
    gun.shootBullet(grid, monsterPos);
}