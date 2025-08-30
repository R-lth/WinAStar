#include "Game.h"

void Game::init(HWND hWnd)
{
    // 화면 크기
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    // 그리드 초기화
    setPlayGround();

    // 플레이어 위치 초기화
    player.setPos({ 10,10 });

    // 몬스터 위치 초기화 관련
    srand(time(NULL));

    // TODO. 타이머
    SetTimer(hWnd, 1, 150 * 2, NULL);
    SetTimer(hWnd, 2, 1500, NULL);
    SetTimer(hWnd, 3, 150, NULL);
}

void Game::update(HWND hWnd, WPARAM wParam)
{
    // TODO. 타이머 및 입풋 처리
    switch (wParam)
    {
    case 1:
    {
        for (int id = 0; id < monsterPos.size(); ++id)
        {
            POINT next = monsterPos[id];

            while (!pathInfo[id].empty() && monsterPos[id].x == next.x && monsterPos[id].y == next.y)
            {
                next = pathInfo[id].front();
                pathInfo[id].pop_front();
            }

            if (!isInRange(monsterPos[id]) || isObstacle(monsterPos[id]))
            {
                continue;
            }

            if (CollideWithOtherMonsters(id, monsterPos[id]))
            {
                continue;
            }

            // 키가 눌리지 않아도, 플레이어와 몬스터 간의 충돌이 가능하도록
            if (CollideWithPlayer(monsterPos[id]))
            {
                isWaiting = true;
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
                KillTimer(hWnd, 3);

                // 2초 후 
                SetTimer(hWnd, 4, 2000, NULL);
                break;
            }
            else
            {
                // TODO. A*의 대각선 {x, y} 값 고려하기
                monsterPos[id] = next;
                deque<POINT> path = aStar.findPath(monsterPos[id], player, playGrid);
                pathInfo[id] = path;
            }
        }
    }
    break;
    case 2:
    {
        // 몬스터 생성
        const vector<POINT> center =
        {
            {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12},
            {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12},
            {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0},
            {7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}
        };

        int i = rand() % center.size();

        POINT monster = center[i];

        if (isInRange(monster) && !isObstacle(monster) && !CollideWithPlayer(monster))
        {
            deque<POINT> path = aStar.findPath(monster, player, playGrid);
            pathInfo.emplace_back(path);
            int id = pathInfo.size() - 1;
            monsterPos.insert({ id, monster });
        }
    }
    break;
    case 3:
    {
        // TODO. 총알 위치 갱신 및 피격 판정
        using It = list<pair<int, POINT>>::iterator;
        for (It it = gun.begin(); it != gun.end();)
        {
            // 반복자 위치 갱신
            POINT bullet = it->second;

            switch (it->first)
            {
            case 0:
                bullet.x -= 1;
                bullet.y -= 1;
                break;
            case 1:
                bullet.x += 1;
                bullet.y -= 1;
                break;
            case 2:
                bullet.x -= 1;
                bullet.y += 1;
                break;
            case 3:
                bullet.x += 1;
                bullet.y += 1;
                break;
            case 4:
                bullet.x -= 1;
                break;
            case 5:
                bullet.x += 1;
                break;
            case 6:
                bullet.y -= 1;
                break;
            case 7:
                bullet.y += 1;
                break;
            default:
                break;
            }

            // 1. 범위, 장애물
            if (!isInRange(bullet) || isObstacle(bullet))
            {
                // 제거
                it = gun.erase(it);
                continue;
            }

            // 2. 사격 판정
            bool hit = false;

            for (int id = 0; id < monsterPos.size(); ++id)
            {
                if (bullet.x == monsterPos[id].x && bullet.y == monsterPos[id].y)
                {
                    hit = true;
                    monsterPos.erase(id);
                    it = gun.erase(it);
                    break;
                }
            }

            if (!hit)
            {
                it->second = bullet;
                it = next(it);
            }
        }
    }
    break;
    case 4:
    {

        KillTimer(hWnd, 4);

        if (isWaiting)
        {
            isWaiting = false;
            // 게임 종료 
            gameOver = true;

            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
            // TODO. 바로 업데이트
            UpdateWindow(hWnd);
        }
    }
    break;
    default:
        break;
    }

    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

void Game::render(HDC hdc, HINSTANCE hInst)
{
	renderBegin(hdc, hInst);
    renderPlay();
    // 실제 화면 출력
    BitBlt(hdc, 0, 0, width, height, back, 0, 0, SRCCOPY);
	renderEnd();
}

void Game::renderBegin(HDC hdc, HINSTANCE hInst)
{
    back = CreateCompatibleDC(hdc);
    scr = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, width, height); 
    originalBmp = (HBITMAP)SelectObject(back, bmp);

    // 땅
    groundBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AISLE));
    groundBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BRICK));
    // 
    blackBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACK));
    // UI 텍스터
    uiTextBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH1));
    uiTextBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH2));
    uiTextBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH3));
    uiTextBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH4));
    uiTextBmp[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH5));
    uiTextBmp[5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH6));
    uiTextBmp[6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH7));
    
    // 플레이어
    playerBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOAL));
    playerBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALLEFT));
    playerBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALRIGHT));
    playerBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALUP));
    // 총
    bulletBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BULLET));
    // 몬스터
    monsterBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER1));
    monsterBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER2));
    // 죽음
    deadBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEAD));
}

void Game::renderPlay()
{
    if (!gameOver) 
    {
        for (int y = 0; y < n; ++y)
        {
            for (int x = 0; x < n; ++x)
            {
                HBITMAP tile = grid[y][x] ? groundBmp[1] : groundBmp[0];
                SelectObject(scr, tile);
                BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }
        }

        if (!waiting)
        {
            // 플레이어
            if (pHoriz)
            {
                HBITMAP pSprite = pFilp ? playerBmp[1] : playerBmp[2];
                SelectObject(scr, pSprite);
            }
            else
            {
                HBITMAP pSprite = pUp ? playerBmp[3] : playerBmp[0];
                SelectObject(scr, pSprite);
            }
            BitBlt(back, playerPos.x * cell, playerPos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);

            // 몬스터
            HBITMAP mSprite = mFilp ? monsterBmp[0] : monsterBmp[1];
            mFilp = !mFilp;
            SelectObject(scr, mSprite);

            for (const pair<int, POINT>& it : monsterPos)
            {
                POINT pos = it.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // 총알
            SelectObject(scr, bulletBmp);
            using It = list<pair<int, POINT>>::iterator;
            for (It it = gun.begin(); it != gun.end();)
            {
                POINT bullet = it->second;
                BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                it = next(it);
            }
        }
        else
        {
            // 플레이어 죽음
            SelectObject(scr, deadBmp);
            BitBlt(back, playerPos.x * cell, playerPos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);

            // 몬스터 죽음
            SelectObject(scr, deadBmp);
            for (const pair<int, POINT>& it : monsterPos)
            {
                POINT pos = it.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // 총알 지우기
            SelectObject(scr, groundBmp[0]);
            using It = list<pair<int, POINT>>::iterator;
            for (It it = gun.begin(); it != gun.end();)
            {
                POINT bullet = it->second;
                BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                it = next(it);
            }
        }
    }
    else 
    {
        for (int y = 0; y < n; ++y)
        {
            for (int x = 0; x < n; ++x)
            {
                SelectObject(scr, blackBmp);
                BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }
        }

        for (int i = 0; i < uiSpawn.size(); ++i)
        {
            SelectObject(scr, uiTextBmp[i]);

            int x = uiSpawn[i].x;
            int y = uiSpawn[i].y;
            BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
        }
    }
}

void Game::renderEnd()
{
    // 메모리 누수 방지
    SelectObject(back, originalBmp);
    // 메모리 해제
    // 땅
    DeleteObject(groundBmp[0]);
    DeleteObject(groundBmp[1]);
    // 
    DeleteObject(blackBmp);
    // 
    DeleteObject(uiTextBmp[0]);
    DeleteObject(uiTextBmp[1]);
    DeleteObject(uiTextBmp[2]);
    DeleteObject(uiTextBmp[3]);
    DeleteObject(uiTextBmp[4]);
    DeleteObject(uiTextBmp[5]);
    DeleteObject(uiTextBmp[6]);
    // 
    DeleteObject(playerBmp[0]);
    DeleteObject(playerBmp[1]);
    DeleteObject(playerBmp[2]);
    DeleteObject(playerBmp[3]);
    //
    DeleteObject(bulletBmp);
    //
    DeleteObject(monsterBmp[0]);
    DeleteObject(monsterBmp[1]);
    //    
    DeleteObject(deadBmp);
    /////////////////
    DeleteObject(bmp);
    DeleteObject(originalBmp);
    DeleteDC(back);
    DeleteDC(scr);
}

void Game::setPlayGround()
{
    const int n = gameState.n;
    gameState.grid.resize(n, vector<int>(n, 0));

    // 모서리와 통로
    for (int i = 0; i < n; ++i)
    {
        int v = (i >= 7 && i < 13) ? 0 : 1;
        gameState.grid[0][i] = v;
        gameState.grid[19][i] = v;
        gameState.grid[i][0] = v;
        gameState.grid[i][19] = v;
    }
}
