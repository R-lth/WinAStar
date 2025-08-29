#include "Game.h"

void Game::init()
{
    // ȭ�� ũ��
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    // �׸��� �ʱ�ȭ
	grid = gridGround();

    // �÷��̾� ��ġ �ʱ�ȭ
    playerPos = { 10,10 };
}

void Game::update()
{
    // TODO. Ÿ�̸� �� ��ǲ ó��
}

void Game::render(HDC hdc, HINSTANCE hInst)
{
	renderBegin(hdc, hInst);
    renderPlay();
    // ���� ȭ�� ���
    BitBlt(hdc, 0, 0, width, height, back, 0, 0, SRCCOPY);
	renderEnd();
}

void Game::renderBegin(HDC hdc, HINSTANCE hInst)
{
    back = CreateCompatibleDC(hdc);
    scr = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, width, height); 
    originalBmp = (HBITMAP)SelectObject(back, bmp);

    // ��
    groundBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AISLE));
    groundBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BRICK));
    // 
    blackBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACK));
    // UI �ؽ���
    uiTextBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH1));
    uiTextBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH2));
    uiTextBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH3));
    uiTextBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH4));
    uiTextBmp[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH5));
    uiTextBmp[5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH6));
    uiTextBmp[6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH7));
    
    // �÷��̾�
    playerBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOAL));
    playerBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALLEFT));
    playerBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALRIGHT));
    playerBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALUP));
    // ��
    bulletBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BULLET));
    // ����
    monsterBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER1));
    monsterBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER2));
    // ����
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
            // �÷��̾�
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

            // ����
            HBITMAP mSprite = mFilp ? monsterBmp[0] : monsterBmp[1];
            mFilp = !mFilp;
            SelectObject(scr, mSprite);

            for (const pair<int, POINT>& it : monsterPos)
            {
                POINT pos = it.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // �Ѿ�
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
            // �÷��̾� ����
            SelectObject(scr, deadBmp);
            BitBlt(back, playerPos.x * cell, playerPos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);

            // ���� ����
            SelectObject(scr, deadBmp);
            for (const pair<int, POINT>& it : monsterPos)
            {
                POINT pos = it.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // �Ѿ� �����
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
    // �޸� ���� ����
    SelectObject(back, originalBmp);
    // �޸� ����
    // ��
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
