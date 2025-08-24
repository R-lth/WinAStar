#pragma once
#include <Windows.h>

#include "AStar.h"

class Game
{
public:
	void Init(HWND hWnd);
	void Update();
	void Render();

	void setPos(Position pos);

private:
	void drawGrid();
	void animation();

public:
	Position start;
	Position goal;

private:
	vector<vector<int>> grid;
	AStar aStar;
	stack<Position> path;
};