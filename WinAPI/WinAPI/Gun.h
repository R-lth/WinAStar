#pragma once

#include "windef.h"
#include <vector>
#include <list>
#include "Collision.h"
#include "Types.h"

using namespace std;

class Gun
{
public:
	Gun() = default;
	~Gun() = default;

public:
	void spawnBullet(pair<ShootDir, POINT> p, const vector<vector<int>> grid);
	void shootBullet(const vector<vector<int>> grid, map<int, POINT>& monsterPos);

private:
	list<pair<ShootDir, POINT>> gun;
	Collision collision;
};

