#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>

using namespace std;

// 2D 격자
using Position = std::pair<int, int>;

// TODO. 구조체, 연산자 오버로딩
// TODO. 사용자 정의 해시
struct Node
{
	Position current;	// 현재 위치
	Position parent;	// 이전 위치

	float gCost;		
	float hCost;
};

class AStar
{
public:
	void setPath(Position current);

	stack<Position> getPath();

	stack<Position> findPath(Position start, Position goal, const std::vector<std::vector<int>>& grid);

private:
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

	priority_queue<Node, vector<Node>, greater<Node>> pq;
	unordered_map<Position, Position> visited; 
	stack<Position> path;
};