#pragma once

#include <windows.h>
#include <vector>
#include <queue>
#include <map>

using namespace std;

struct Node
{
	POINT current;
	POINT parent;
	float gCost;
	float hCost;
};

inline const bool operator< (const POINT& lhs, const POINT& rhs)
{
	return (lhs.x == rhs.x) ? lhs.y < rhs.y : lhs.x < rhs.x;
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
	vector<POINT> findPath(POINT start, POINT goal, vector<vector<char>> grid);

private:
	void setPath(POINT moving);
	float heuristic(POINT next, POINT goal);

private:
	const std::vector<pair<POINT, float>> direction =
	{
		{ {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
		{ {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
	};

	vector<POINT> path = {};
	map<POINT, POINT> visited;
};