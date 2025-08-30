#pragma once

#include <vector>
#include <queue>
#include <map>
#include <deque>
#include "GameState.h"
#include "Types.h"

using namespace std;

struct Node
{
	Position current;
	Position parent;
	float gCost;
	float hCost;
};

inline const bool operator< (const Position& a, const Position& b)
{
	return (a.x == b.x) ? a.y < b.y : a.x < b.x;
}

struct Compare
{
	bool operator() (const Node& a, const Node& b)
	{
		return (a.gCost + a.hCost) > (b.gCost + b.hCost);
	}
};

class AStar
{
public:
	deque<Position> findPath(Position start, Position goal);

private:
	deque<Position> getPath(Position character, map<Position, Position> visited);
	float heuristic(Position next, Position goal);

private:
	const std::vector<pair<Position, float>> direction =
	{
		{ {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
		{ {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
	};

	GameState gameState;
};