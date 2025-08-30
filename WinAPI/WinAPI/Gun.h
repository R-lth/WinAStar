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
	list<pair<ShootDir, Position>>& getGun();
	void loadingBullets(pair<ShootDir, Position> p);
	void shootBullets();

private:
	list<pair<ShootDir, Position>> gun;
	Collision collision;
};

