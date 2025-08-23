#pragma once

#include <Windows.h>

class Game
{
public:
	Game() = default;
	~Game() = default;

public:
	void Init(HWND hWnd);
	void Update();
	void Render();
};