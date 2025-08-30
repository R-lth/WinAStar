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
	void loadingBullets(pair<ShootDir, POINT> p);
	void shootBullets();

private:
	list<pair<ShootDir, POINT>> gun;
	Collision collision;
};

