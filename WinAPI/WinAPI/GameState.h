#pragma once

// 게임 상태
bool gameOver = false;
bool waiting = false;

// 몬스터 정보
AStar aStar;
vector<deque<POINT>> pathInfo;
map<int, POINT> monsterPos;