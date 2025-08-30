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
    switch (wParam)
    {
    case 1:
    {
        Position pos;

        if (!gameState.gameOver && !gameState.waiting) 
        {
            pos = player.movePlayer(hWnd);
        }

        monster.moveMonster(pos);
        gun.shootBullets();
    }
    break;
    case 2:
    {
        monster.spawnMonster(player.getPos());
    }
    break;
    case 3:
    {
        if (!gameState.gameOver && !gameState.waiting)
        {
            player.shoot();
        }
    }
    break;
    case 4:
    {
        KillTimer(hWnd, 4);

        if (gameState.waiting)
        {
            gameState.waiting = false;
            gameState.gameOver = true;

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
    if (!gameState.gameOver) 
    {
        for (int y = 0; y < gameState.n; ++y)
        {
            for (int x = 0; x < gameState.n; ++x)
            {
                HBITMAP tile = gameState.grid[y][x] ? groundBmp[1] : groundBmp[0];
                SelectObject(scr, tile);
                BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }
        }

        if (!gameState.waiting)
        {
            // 플레이어
            if (player.getHorizontal())
            {
                HBITMAP pSprite = player.getFilp() ? playerBmp[1] : playerBmp[2];
                SelectObject(scr, pSprite);
            }
            else
            {
                HBITMAP pSprite = player.getVertical() ? playerBmp[3] : playerBmp[0];
                SelectObject(scr, pSprite);
            }
            BitBlt(back, player.getPos().x * cell, player.getPos().y * cell, cell, cell, scr, 0, 0, SRCCOPY);

            // 몬스터
            HBITMAP mSprite = monster.getMFlip() ? monsterBmp[0] : monsterBmp[1];
            monster.setMFilp();
            SelectObject(scr, mSprite);

            for (const pair<int, Position>& it : gameState.monsterPos)
            {
                Position pos = it.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // 총알
            SelectObject(scr, bulletBmp);
            using It = list<pair<ShootDir, Position>>::iterator;
            for (It it = gun.getGun().begin(); it != gun.getGun().end();)
            {
                Position bullet = it->second;
                BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                it = next(it);
            }
        }
        else
        {
            // 플레이어 죽음
            SelectObject(scr, deadBmp);
            BitBlt(back, player.getPos().x * cell, player.getPos().y * cell, cell, cell, scr, 0, 0, SRCCOPY);

            // 몬스터 죽음
            SelectObject(scr, deadBmp);
            for (const pair<int, Position>& it : gameState.monsterPos)
            {
                Position pos = it.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // 총알 지우기
            SelectObject(scr, groundBmp[0]);
            using It = list<pair<ShootDir, Position>>::iterator;
            for (It it = gun.getGun().begin(); it != gun.getGun().end();)
            {
                Position bullet = it->second;
                BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                it = next(it);
            }
        }
    }
    else 
    {
        for (int y = 0; y < gameState.n; ++y)
        {
            for (int x = 0; x < gameState.n; ++x)
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
