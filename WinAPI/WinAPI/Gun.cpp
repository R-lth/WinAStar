#include "Gun.h"

list<pair<ShootDir, Position>>& Gun::getGun()
{
    return gun;
}

void Gun::loadingBullets(pair<ShootDir, Position> p)
{
    if (collision.okToGo(p.second))
    {
        gun.push_back({p.first, p.second });
    }
}

void Gun::shootBullets()
{
    using It = list<pair<ShootDir, Position>>::iterator;
    for (It it = gun.begin(); it != gun.end();)
    {
        Position next = it->second;
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

        bool hit = collision.shot(next, it);

        if (!hit)
        {
            it->second = next;
            it = ::next(it);
        }
    }
}
