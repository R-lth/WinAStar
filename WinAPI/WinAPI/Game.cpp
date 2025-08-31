#include "Game.h"
#include "Resource.h"

void Game::init(RECT rect)
{
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

void Game::render(HDC hdc, HINSTANCE hInst,
    bool gameOver, bool waiting, bool pHoriz, bool pUp, bool pFilp, 
        POINT playerPos, map<int, POINT> monsterPos,
        const vector<vector<int>> grid, list<pair<int, POINT>> gun)
{
    // 로드
    renderBegin(hdc, hInst);
    // 그리기
    renderPlay(gameOver, waiting, pHoriz, pUp, pFilp, 
        playerPos, monsterPos,
        grid, gun);
    // 실제 화면 출력
    BitBlt(hdc, 0, 0, width, height, back, 0, 0, SRCCOPY);
    // 해제
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

void Game::renderPlay(bool gameOver, bool waiting, bool pHoriz, bool pUp, bool pFilp, 
    POINT playerPos, map<int, POINT> monsterPos,
                            const vector<vector<int>> grid, list<pair<int, POINT>> gun)
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
            if (i < 5)
            {
                SelectObject(scr, uiTextBmp[i]);
            }
            else if (i == 5) 
            {
                SelectObject(scr, uiTextBmp[3]);
            }
            else if (i > 5)
            {
                SelectObject(scr, uiTextBmp[i - 1]);
            }

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
    for (int i = 0; i < 2; ++i)
    {
        DeleteObject(groundBmp[i]);
    }

    DeleteObject(blackBmp);

    for (int i = 0; i < 7; ++i) 
    {
        DeleteObject(uiTextBmp[i]);
    }

    for (int i = 0; i < 4; ++i) 
    {
        DeleteObject(playerBmp[i]);
    }

    DeleteObject(bulletBmp);

    for (int i = 0; i < 2; ++i) 
    {
        DeleteObject(monsterBmp[i]);
    }

    DeleteObject(deadBmp);
    /////////////////
    DeleteObject(bmp);
    DeleteObject(originalBmp);
    DeleteDC(back);
    DeleteDC(scr);
}