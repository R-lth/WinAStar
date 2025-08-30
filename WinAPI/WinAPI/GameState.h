#pragma once

#include "AStar.h"

struct GameState
{
	// 게임 상태
	bool gameOver = false;
	bool waiting = false;

	// 2D 타일맵
	const int n = 20;
	vector<vector<int>>& grid;

	// 몬스터 정보
	AStar aStar;
	vector<deque<POINT>> pathInfo;
	map<int, POINT> monsterPos;
};

