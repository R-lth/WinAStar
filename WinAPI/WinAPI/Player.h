#pragma once

// TODO. �ٵ� ���������� ������ �ǵ� winproc �Լ� ��������
// �����ϴ� �� �ƴ�??????????????????????

#include "windef.h"
#include <vector>
#include <map>
#include <deque>
#include <list>

#include "Types.h"
#include "Gun.h"
#include "Collision.h"

using namespace std;

class Player
{
public:
	Player() = default;
	~Player() = default;
	
public:
    // TODO. grid�ϰ� monsterPos�� �������� ó���ϴ� �� ������
    // �ƴϸ� Game���� å������ �� ������
	void movePlayer(const vector<vector<int>>& grid, map<int, POINT>& monsterPos);
    void shoot(const vector<vector<int>> grid, map<int, POINT>& monsterPos);

private:
	POINT pos;
    bool pHoriz = false;
    bool pFilp = false;
    bool pUp = false;
    Collision collision;
    Gun gun;
};

