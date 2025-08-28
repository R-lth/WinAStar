// WinAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI.h"

#include <unordered_map>
#include <queue>
#include <deque>
#include <random>

#include "pch.h"
#include "AStar.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;                                      // 창 핸들 전역으로 초기화.

#pragma region 전역
RECT rect;

AStar aStar;
POINT player = { 10,10 };
// index : 몬스터 ID, 값 : 몬스터 경로
vector<deque<POINT>> pathInfo;
// index : 몬스터 ID, 값 : 몬스터 현재 위치
vector<POINT> monsterPos;

// 0 : 길, 1 : 벽
vector<vector<int>> playGrid(20, vector<int>(20, 0));
vector<vector<int>> endGrid(20, vector<int>(20, 0));

const std::vector<pair<POINT, float>> direction =
{
    { {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
    { {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
};

const int row = static_cast<int>(playGrid.size());
const int column = static_cast<int>(playGrid[0].size());
const int cell = 20;

bool isInRange(POINT pos)
{
    return (pos.x >= 0 && pos.x < column && pos.y >= 0 && pos.y < row);
}

bool isObstacle(POINT pos)
{
    return (playGrid[pos.y][pos.x]);
}

bool CollideWithOtherMonsters(int id, POINT pos)
{
    for (int i = 0; i < monsterPos.size(); ++i) 
    {
        if (id == i) 
        {
            continue;
        }

        POINT other = monsterPos[i];
        if (pos.x == other.x && pos.y == other.y) 
        {
            return true;
        }
    }

    return false;
}

bool CollideWithPlayer(POINT pos) 
{
    return (pos.x == player.x && pos.y == player.y);
}

bool CanSpawn(POINT pos)
{
    if (pos.x < 0 || pos.x >= column || pos.y < 0 || pos.y >= row)
    {
        return false;
    }

    if (playGrid[pos.y][pos.x])
    {
        return false;
    }

    for (const deque<POINT>& monsterPath : pathInfo) 
    {
        for (const POINT& monsterPos : monsterPath) 
        {
            if (pos.x == monsterPos.x && pos.y == monsterPos.y)
            {
                return false;
            }
        }
    }

    return true;
}

bool mFilp = false;

bool pHoriz = false;
bool pFilp = false;
bool pUp = false;

bool gameOver = false;
bool isWaiting = false;
#pragma endregion

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    // 1. 윈도우 창 정보 등록
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    // 2. 윈도우 창 생성
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg = {};

    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            int a = 0;
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL; // 메뉴바 지움
    wcex.lpszClassName  = L"kohmeso";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
// 
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 창 크기 설정
   rect = { 0, 0, GWinSizeX, GWinSizeY }; // 800, 600
   ::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

   hWnd = CreateWindowW(L"kohmeso", L"Client", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ::ShowWindow(hWnd, nCmdShow);
   ::UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int width = GWinSizeX;
    int height = GWinSizeY;

    PAINTSTRUCT ps;
    HDC hdc, back, scr;
    HBITMAP bmp, connect;
    HBITMAP Aisle, Brick, Character1, Character2, Start, Goal, GoalLeft, GoalRight, GoalUp;
    HBITMAP Dead, Black, Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7;

    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_CREATE:
        {
            // 화면 크기
            width = rect.right - rect.left;
            height = rect.bottom - rect.top;

            // 그리드 설정
            for (int i = 0; i < 20; ++i)
            {
                playGrid[0][i] = 1;
                playGrid[19][i] = 1;
                playGrid[i][0] = 1;
                playGrid[i][19] = 1;
            }

            for (int i = 7; i < 13; ++i)
            {
                playGrid[0][i] = 0;
                playGrid[19][i] = 0;
                playGrid[i][0] = 0;
                playGrid[i][19] = 0;
            }

            // 몬스터 생성 위치 관련 랜덤 값을 위한 함수
            srand(time(NULL));

            // 타이머
            SetTimer(hWnd, 1, 150 * 2, NULL);
            SetTimer(hWnd, 2, 1500, NULL);
        }
        break;
        case WM_TIMER:
        {
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
                   
                    // 2. 충돌 처리
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

                        // 2초 후 
                        SetTimer(hWnd, 3, 2000, NULL);
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
                const vector<POINT> center =
                {
                    {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12},
                    {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12},
                    {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0},
                    {7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}
                };

                int i = rand() % center.size();

                POINT monster = center[i];

                if (CanSpawn(monster))
                {
                    deque<POINT> path = aStar.findPath(monster, player, playGrid);
                    pathInfo.emplace_back(path);
                    monsterPos.emplace_back(monster);
                }
            }
                break;
            case 3:
                {
                    KillTimer(hWnd, 3);

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
        break;
        case WM_KEYDOWN:  
        {
            // TODO. Game 객체로 프레임으로 입력 처리를 받아서, 자연스러운 입력 구현
            POINT next = player;

            // TODO. 이동 키 WASD, 사격 키 ← → ↑ ↓
            bool a = GetAsyncKeyState(0x41) & 0x8000;
            bool d = GetAsyncKeyState(0x44) & 0x8000;
            bool w = GetAsyncKeyState(0x57) & 0x8000;
            bool s = GetAsyncKeyState(0x53) & 0x8000;

            bool left = GetAsyncKeyState(VK_LEFT) & 0x8000;
            bool right = GetAsyncKeyState(VK_RIGHT) & 0x8000;
            bool up = GetAsyncKeyState(VK_UP) & 0x8000;
            bool down = GetAsyncKeyState(VK_DOWN) & 0x8000;

            if ((a || left) && (w || up)) 
            {
                next.x -= 1;
                next.y -= 1;
                // 
                pHoriz = false;
                pUp = true;
            }
            else if ((d || right) && (w || up)) 
            {
                next.x += 1;
                next.y -= 1;
                //
                pHoriz = false;
                pUp = true;
            }
            else if ((a || left) && (s || down)) 
            {
                next.x -= 1;
                next.y += 1;
                //
                pHoriz = false;
                pUp = false;
            }
            else if ((d || right) && (s || down)) 
            {
                next.x += 1;
                next.y += 1;
                // 
                pHoriz = false;
                pUp = false;
            }
            else if (a || left) 
            {
                next.x -= 1;
                pHoriz = true;
                pFilp = !pFilp;
            }
            else if (d || right) 
            {
                next.x += 1;
                pHoriz = true;
                pFilp = !pFilp;
            }
            else if (w || up) 
            {
                next.y -= 1;
                pHoriz = false;
                pUp = true;
            }
            else if (s || down) 
            {
                next.y += 1;
                pHoriz = false;
                pUp = false;
            }

            // 충돌 처리
            if (!isInRange(next) || isObstacle(next))
            {
                break;
            }

            player = next;

            // 플레이어 이동에 따른 몬스터 경로 갱신
            for (int id = 0; id < monsterPos.size(); ++id) 
            {
                deque<POINT> path = aStar.findPath(monsterPos[id], player, playGrid);
                pathInfo[id] = path;     
            }

            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        }
        break;
        case WM_PAINT:
        {
            // 앞면 버퍼. 실제 화면
            hdc = BeginPaint(hWnd, &ps);

#pragma region Render
            // 실제 화면과 호환되는 후면 버퍼. 실제 화면에 제출할 DC 
            back = CreateCompatibleDC(hdc);
            bmp = CreateCompatibleBitmap(hdc, width, height); // dc와 호환되는 비트맵
            connect = (HBITMAP)SelectObject(back, bmp);

            // 리소스 DC 생성
            scr = CreateCompatibleDC(hdc);

            // 비트맵 로드
            Aisle = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AISLE));
            Brick = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BRICK));
            // 총알 = 몬스터
            Character1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER1));
            Character2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER2));
            // 플레이어
            Goal = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOAL));
            GoalLeft = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALLEFT));
            GoalRight = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALRIGHT));
            GoalUp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALUP));
            Dead = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEAD));
            Black = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACK));
            Ch1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH1));
            Ch2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH2));
            Ch3 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH3));
            Ch4 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH4));
            Ch5 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH5));
            Ch6 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH6));
            Ch7 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH7));

            /////////////////////////////////////////////////////////

            //TODO. 격자 그리기 > 2초 공백 후 게임 종료 화면
            for (int y = 0; y < row; ++y)
            {
                for (int x = 0; x < column; ++x)
                {
                    HBITMAP tile = playGrid[y][x] ? Brick : Aisle;
                    SelectObject(scr, tile);
                    BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }
            }

            if (!gameOver && !isWaiting) 
            {
                // 플레이어 표시
                if (pHoriz)
                {
                    HBITMAP pSprite = pFilp ? GoalLeft : GoalRight;
                    SelectObject(scr, pSprite);
                }
                else
                {
                    HBITMAP pSprite = pUp ? GoalUp : Goal;
                    SelectObject(scr, pSprite);
                }
                BitBlt(back, player.x * cell, player.y * cell, cell, cell, scr, 0, 0, SRCCOPY);

                // 장애물 표시
                HBITMAP mSprite = mFilp ? Character1 : Character2;
                SelectObject(scr, mSprite);
                mFilp = !mFilp;

                for (const POINT& pos : monsterPos)
                {
                    BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }
            }
            if (!gameOver && isWaiting)
            {
                // 플레이어 표시
                SelectObject(scr, Dead);
                BitBlt(back, player.x * cell, player.y * cell, cell, cell, scr, 0, 0, SRCCOPY);

                // 장애물 표시
                SelectObject(scr, Dead);
                for (const POINT& pos : monsterPos)
                {
                    BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }
            }
            else if (gameOver)
            {
                // 게임 종료 화면
                for (int y = 0; y < row; ++y)
                {
                    for (int x = 0; x < column; ++x)
                    {
                        SelectObject(scr, Black);
                        BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                    }
                }

                SelectObject(scr, Ch1);
                BitBlt(back, 9 * cell, 6 * cell, cell, cell, scr, 0, 0, SRCCOPY);
                SelectObject(scr, Ch2);
                BitBlt(back, 10 * cell, 6 * cell, cell, cell, scr, 0, 0, SRCCOPY);
                SelectObject(scr, Ch3);
                BitBlt(back, 11 * cell, 6 * cell, cell, cell, scr, 0, 0, SRCCOPY);
                SelectObject(scr, Ch4);
                BitBlt(back, 12 * cell, 6 * cell, cell, cell, scr, 0, 0, SRCCOPY);
                SelectObject(scr, Ch5);
                BitBlt(back, 10 * cell, 7 * cell, cell, cell, scr, 0, 0, SRCCOPY); 
                SelectObject(scr, Ch4);
                BitBlt(back, 11 * cell, 7 * cell, cell, cell, scr, 0, 0, SRCCOPY);
                SelectObject(scr, Ch6);
                BitBlt(back, 10 * cell, 8  * cell, cell, cell, scr, 0, 0, SRCCOPY);
                SelectObject(scr, Ch7);
                BitBlt(back, 11 * cell, 8 * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // 실제 화면 출력
            BitBlt(hdc, 0, 0, width, height, back, 0, 0, SRCCOPY);
#pragma endregion
            // 해제
            /*
             * DC는 항상 어떤 GDI 객체를 선택하고 있어야 함.
             * 복원 순서
             * ① SelectObject(back, bmp);	  후면 버퍼 DC에 비트맵 붙이기
             * ② 그리기
             * ③ SelectObject(back, connect); 원래대로 복원
             *
             * ① connect = (HBITMAP)SelectObject(back, bmp);
             * → 기존에 back DC에 선택돼 있던 GDI 객체를 connect에 저장해 놓고, bmp를 back DC에 선택해서 그릴 수 있게 한다.
             * → select는 기존 반환값을 전달하되, 정작 새로운 GDI 객체를 선택하게 한다.
            */
            SelectObject(back, connect);
            // 
            DeleteObject(Aisle);
            DeleteObject(Brick);
            DeleteObject(Character1);
            DeleteObject(Character2);
            DeleteObject(Goal);
            DeleteObject(GoalLeft);
            DeleteObject(GoalRight);
            DeleteObject(GoalUp);
            DeleteObject(Dead);
            DeleteObject(Black);
            DeleteObject(Ch1);
            DeleteObject(Ch2);
            DeleteObject(Ch3);
            DeleteObject(Ch4);
            DeleteObject(Ch5);
            DeleteObject(Ch6);
            DeleteObject(Ch7);
            //
            DeleteObject(bmp);
            DeleteObject(connect);
            //
            DeleteDC(back);
            DeleteDC(scr);

            // 실제 화면에 최종 제출
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
        return 0;
}

