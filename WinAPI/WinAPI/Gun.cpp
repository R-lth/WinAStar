#include "Gun.h"

void Gun::spawnBullet(pair<ShootDir, POINT> p)
{
    if (collision.okToGo(p.second))
    {
        gun.push_back({p.first, p.second });
    }
}

void Gun::shootBullet()
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

        if (!collision.okToGo(next))
        {
            it = gun.erase(it);
            continue;
        }

        bool hit = false;

        // TODO. collision 컴포넌트의 함수로 만들어야 하는가
        for (int id = 0; id < collision.getMonsterPos().size(); ++id)
        {
            if (next.x == collision.getMonsterPos()[id].x && next.y == collision.getMonsterPos()[id].y)
            {
                //
                it = gun.erase(it);
                //
                collision.getMonsterPos().erase(id);
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
