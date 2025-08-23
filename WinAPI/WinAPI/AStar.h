#pragma once

#include <vector>
#include <queue>
#include <map>
#include <stack>

using namespace std;

// 2D 격자
using Position = std::pair<int, int>;

struct Node 
{
	Position current;
	Position parent;
	float gCost = 0.f;
	float hCost = 0.f;
	float f() const { return gCost + hCost; }
};

// 우선 순위 큐에서 사용자 정의 타입을 오름차순으로 정렬하기 위한, 연산자 오버로딩이다.
struct Compare 
{
	// vector의 greater은 내림차순이지만, priority_queue(heap)의 greater은 오름차순이다.
	bool operator() (const Node& a, const Node& b) const 
	{
		return a.f() > b.f(); 
	}
};

std::priority_queue<Node, std::vector<Node>, Compare> pq;


class AStar
{
public:
	stack<Position> getPath();

	stack<Position> findPath(Position start, Position goal, const std::vector<std::vector<int>>& grid);

private:
	void setPath(Position current);

	bool isInRange(Position pos, const std::vector<std::vector<int>>& grid);

	float heuristic(Position current, Position goal);

private:
	const vector<pair<Position, float>> direction =
		{
			// 하상우좌 이동.
			{ {0, 1}, 1.0f}, { {0, -1}, 1.0f}, { {1, 0}, 1.0f}, { {-1, 0}, 1.0f},
			// 대각선 이동.
			{ {1, 1}, 1.414f}, { {1, -1}, 1.414f}, { {-1, 1}, 1.414f}, { {-1, -1}, 1.414f}
		};

	priority_queue<Node, vector<Node>, Compare> pq;
	map<Position, Position> visited; 
	stack<Position> path;
};