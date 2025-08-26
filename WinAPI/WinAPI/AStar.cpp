#include "AStar.h"

vector<POINT> AStar::findPath(POINT start, POINT goal, const vector<vector<int>> grid)
{
	priority_queue<Node, vector<Node>, Compare> pq;
	visited.clear();
	path.clear();

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
		pq.pop();

		// 이미 방문
		if (visited.find(node.current) != visited.end())
		{
			continue;
		}

		// 이제 방문
		visited.insert({ node.current, node.parent });

		// 목적지 도착
		if (node.current.x == goal.x && node.current.y == goal.y) 
		{
			setPath(node.current);
			return path;
		}

		// 이웃 탐색
		for (const pair<POINT, float>& dir : direction)
		{
			int nextY = node.current.y + dir.first.y;
			int nextX = node.current.x + dir.first.x;
			POINT next = { nextX, nextY };
			
			int row = static_cast<int>(grid.size());
			int col = static_cast<int>(grid[0].size());
			if (nextX < 0 || nextX >= col || nextY < 0 || nextY >= row) 
			{
				continue;
			}

			if (grid[nextY][nextX]) 
			{
				continue;
			}

			if (visited.find(next) != visited.end())
			{
				continue;
			}

			Node neighbor;
			neighbor.current = next;
			neighbor.parent = node.current;
			neighbor.gCost = node.gCost + dir.second;
			neighbor.hCost = heuristic(start, goal);

			pq.push(neighbor);
		}
	}

	// 경로 없음 {}
	return path;
}

void AStar::setPath(POINT current)
{
	POINT point = current;
	while (point.x != -1 || point.y != -1) 
	{
		path.push_back(current);
		point = visited[current];
	}
}

float AStar::heuristic(POINT next, POINT goal)
{
	int diffX = next.x - goal.y;
	int diffY = next.y - goal.y;
	return static_cast<float>(sqrt(diffX * diffX + diffY * diffY));
}
