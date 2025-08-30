#include "Gun.h"

void Gun::spawnBullet(pair<ShootDir, POINT> p, const vector<vector<int>> grid)
{
    if (collision.okToGo(p.second, grid))
    {
        gun.push_back({p.first, p.second });
    }
}

void Gun::shootBullet(const vector<vector<int>> grid, map<int, POINT>& monsterPos)
{
    using It = list<pair<ShootDir, POINT>>::iterator;
    for (It it = gun.begin(); it != gun.end();)
    {
        POINT next = it->second;
        switch (it->first)
        {
        case ShootDir::UpLeft:
            next.x -= 1;
            next.y -= 1;
            break;
        case ShootDir::UpRight:
            next.x += 1;
            next.y -= 1;
            break;
        case ShootDir::DownLeft:
            next.x -= 1;
            next.y += 1;
            break;
        case ShootDir::DownRight:
            next.x += 1;
            next.y += 1;
            break;
        case ShootDir::Up:
            next.y -= 1;
            break;
        case ShootDir::Left:
            next.x -= 1;
            break;
        case ShootDir::Down:
            next.y += 1;
            break;
        case ShootDir::Right:
            next.x += 1;
            break;
        default:
            break;
        }

        if (!collision.okToGo(next, grid))
        {
            it = gun.erase(it);
            continue;
        }

        bool hit = false;

        // TODO. collision 컴포넌트가 피격 판정까지 관리해야 하는가.
        for (int id = 0; id < monsterPos.size(); ++id)
        {
            if (next.x == monsterPos[id].x && next.y == monsterPos[id].y)
            {
                //
                it = gun.erase(it);
                //
                monsterPos.erase(id);
                hit = true;
                break;
            }
        }

        if (!hit)
        {
            it->second = next;
            it = ::next(it);
        }
    }
}
