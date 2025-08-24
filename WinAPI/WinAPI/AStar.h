#pragma once

#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <windef.h>

using namespace std;

struct Node 
{
	POINT current;
	POINT parent;
	float gCost = 0.f;
	float hCost = 0.f;
};

// 우선 순위 큐에서 사용자 정의 타입을 오름차순으로 정렬하기 위한, 연산자 오버로딩이다.
struct Compare 
{
	// vector의 greater은 내림차순이지만, priority_queue(heap)의 greater은 오름차순이다.
	bool operator() (const Node& a, const Node& b) const 
	{
		return (a.gCost + a.hCost) > (a.gCost + a.hCost);
	}
};

class AStar
{
public:
	vector<POINT> findPath(POINT start, POINT goal, const std::vector<std::vector<int>>& grid);

private:
	bool IsDestination(POINT start, POINT goal);
	void setPath(POINT current, map<POINT, POINT>& visited);
	bool isInRange(POINT pos, int row, int column);
	float heuristic(POINT next, POINT goal);

private:
	const vector<pair<POINT, float>> direction =
		{
			// 하상우좌 이동.
			{ {0, 1}, 1.0f}, { {0, -1}, 1.0f}, { {1, 0}, 1.0f}, { {-1, 0}, 1.0f},
			// 대각선 이동.
			{ {1, 1}, 1.414f}, { {1, -1}, 1.414f}, { {-1, 1}, 1.414f}, { {-1, -1}, 1.414f}
		};
	map<POINT, POINT> visited;
	vector<POINT> path = {};
};