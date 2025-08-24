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

/* 연산자 오버로딩
 * → 사용자 정의 타입이므로, 값을 비교해줄 기준을 제공해야 함
 * 
 * 
*/

// 맵의 키에서 사용자 정의 타입을 정렬하기 위한, 연산자 오버로딩
// 맵은 키를 <으로 정렬
//
// 전역 함수이므로 inline으로 중복 정의 방지
inline const bool operator< (const POINT& lhs, const POINT& rhs)
{
	return (lhs.x == rhs.x) ? lhs.y < rhs.y : lhs.x < rhs.x;
}

// 우선 순위 큐에서 사용자 정의 타입을 오름차순으로 정렬하기 위한, 연산자 오버로딩
// 우선 순위 큐는 함수 객체(예, Compare)로 정렬
// 
// 객체 + operator() = 함수처럼 동작하는 객체 = 함수 객체(Functor)
struct Compare 
{
	// vector의 greater은 내림차순이지만, priority_queue(heap)의 greater은 오름차순이다.
	const bool operator() (const Node& a, const Node& b) const 
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