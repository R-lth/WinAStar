#include "AStar.h"

Position AStar::getPos()
{
	return pos;
}

stack<Position> AStar::findPath(Position start, Position goal, const std::vector<std::vector<int>>& grid)
{
	priority_queue<Node, vector<Node>, Compare> pq;
	map<Position, Position> visited;
	stack<Position> path;

	// 초기화
	Node startNode;
	startNode.current = start;
	startNode.parent = { -1, -1 };
	startNode.gCost = 0;
	startNode.hCost = heuristic(start, goal); 

	// 시작
	pq.push(startNode);

	// 탐색
	while (!pq.empty())
	{
		// 최소 값
		Node node = pq.top();
		Position current = { node.current.first, node.current.second };
		pq.pop();

		// TODO. 현재 위치 기록
		pos = current;

		// 이미 방문
		if (visited.find(node.current) != visited.end())
		{
			continue;
		}

		// 이제 방문
		visited.insert({ node.current, node.parent });

		// 목적지 도착
		if (current == goal)
		{
			setPath(path, visited, current);
			return path;
		}

		// 이웃 탐색
		for (const pair<Position, float>& dir : direction)
		{
			int nextY = current.second + dir.first.second;
			int nextX = current.first + dir.first.first;
			Position nextPos = { nextX, nextY };

			if (!isInRange(nextPos, grid) || grid[nextY][nextX] == 1) 
			{
				continue;
			}

			if (visited.find(nextPos) != visited.end())
			{
				continue;
			}

			Node nextNode;
			nextNode.current = nextPos;
			nextNode.parent = current;
			// TODO. 비용 갱신 및 휴리스틱
			nextNode.gCost = node.gCost + dir.second;
			nextNode.hCost = heuristic(nextPos, goal);

			pq.push(nextNode);
		}
	}

	// 경로 없음
	return {};
}

void AStar::setPath(stack<Position>& path, map<Position, Position>& visited, Position current)
{
	Position pos = current;

	while (pos.first != -1 || pos.second != -1)
	{
		path.push(pos);

		// 이동
		map<Position, Position>::iterator it = visited.find(pos);

		if (it != visited.end())
		{
			pos = it->second;
		}
		else
		{
			break;
		}
	}
}

bool AStar::isInRange(Position pos, const std::vector<std::vector<int>>& grid)
{
	return (pos.first >= 0 && pos.first < static_cast<int>(grid[0].size()) &&
		pos.second >= 0 && pos.second < static_cast<int>(grid.size()));
}

float AStar::heuristic(Position current, Position goal)
{
	int diffX = current.first - goal.first;
	int diffY = current.second - goal.second;
	return static_cast<float>(sqrt(diffX * diffX + diffY * diffY));
}
