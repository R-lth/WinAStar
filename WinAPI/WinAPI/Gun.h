#pragma once

#include <list>
#include "Types.h"
#include "Collision.h"

using namespace std;

class Gun
{
public:
	Gun() = default;
	~Gun() = default;

public:
	void spawnBullet(pair<ShootDir, POINT> p);
	void shootBullet();

private:
	list<pair<ShootDir, POINT>> gun;
	Collision collision;
};

