// WinAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI.h"

#include <map>
#include <queue>
#include <deque>
#include <list>
#include <random>

#include "pch.h"
#include "AStar.h"
#include "Game.h"
#include "GameState.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;                                      // 창 핸들 전역으로 초기화.
Game game;

#pragma region 전역
RECT rect;

AStar aStar;
// index : 몬스터 ID, 값 : 몬스터 경로
vector<deque<POINT>> pathInfo;

const std::vector<pair<POINT, float>> direction =
{
    { {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
    { {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
};

bool isInRange(POINT pos)
{
    return (pos.x >= 0 && pos.x < 20 && pos.y >= 0 && pos.y < 20);
}

bool isObstacle(POINT pos)
{
    return (GameState::Get().grid[pos.y][pos.x]);
}

bool CollideWithOtherMonsters(int id, POINT pos)
{
    for (const pair<int, POINT>& it : GameState::Get().monsterPos)
    {
        if (id == it.first) 
        {
            continue;
        }

        POINT other = it.second;
        if (pos.x == other.x && pos.y == other.y)
        {
            return true;
        }
    }

    return false;
}

bool CollideWithPlayer(POINT pos) 
{
    return (pos.x == GameState::Get().playerPos.x && pos.y == GameState::Get().playerPos.y);
}

bool CollideWithAllMonsters(POINT pos) 
{
    for (const pair<int, POINT>& it : GameState::Get().monsterPos)
    {
        POINT monster = it.second;
        if (pos.x == monster.x && pos.y == monster.y)
        {
            return true;
        }
    }

    return false;
}

bool CanSpawn(POINT pos)
{
    if (pos.x < 0 || pos.x >= 20 || pos.y < 0 || pos.y >= 20)
    {
        return false;
    }

    if (GameState::Get().grid[pos.y][pos.x])
    {
        return false;
    }

    for (const deque<POINT>& path : pathInfo) 
    {
        for (const POINT& monster : path) 
        {
            if (pos.x == monster.x && pos.y == monster.y)
            {
                return false;
            }
        }
    }

    return true;
}
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
    game.init(rect);

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
    PAINTSTRUCT ps;
    HDC hdc;

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

            // 그리드 설정
            for (int i = 0; i < 20; ++i)
            {
                GameState::Get().grid[0][i] = 1;
                GameState::Get().grid[19][i] = 1;
                GameState::Get().grid[i][0] = 1;
                GameState::Get().grid[i][19] = 1;
            }

            for (int i = 7; i < 13; ++i)
            {
                GameState::Get().grid[0][i] = 0;
                GameState::Get().grid[19][i] = 0;
                GameState::Get().grid[i][0] = 0;
                GameState::Get().grid[i][19] = 0;
            }

            // 몬스터 생성 위치 관련 랜덤 값을 위한 함수
            srand(time(NULL));

            // 타이머
            SetTimer(hWnd, 1, 150 * 2, NULL);
            SetTimer(hWnd, 2, 1500, NULL);
            SetTimer(hWnd, 3, 150, NULL);
        }
        break;
        case WM_TIMER:
        {
            switch (wParam)
            {
            case 1:
            {
                for (int id = 0; id < GameState::Get().monsterPos.size(); ++id)
                {
                    POINT next = GameState::Get().monsterPos[id];

                    while (!pathInfo[id].empty() && GameState::Get().monsterPos[id].x == next.x && GameState::Get().monsterPos[id].y == next.y)
                    {   
                        next = pathInfo[id].front();
                        pathInfo[id].pop_front();
                    }
                    
                    if (!isInRange(GameState::Get().monsterPos[id]) || isObstacle(GameState::Get().monsterPos[id]))
                    {
                        continue;
                    }
                    
                    if (CollideWithOtherMonsters(id, GameState::Get().monsterPos[id]))
                    {
                        continue;
                    }

                    // 키가 눌리지 않아도, 플레이어와 몬스터 간의 충돌이 가능하도록
                    if (CollideWithPlayer(GameState::Get().monsterPos[id]))
                    {
                        GameState::Get().waiting = true;
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
                        GameState::Get().monsterPos[id] = next;
                        deque<POINT> path = aStar.findPath(GameState::Get().monsterPos[id], GameState::Get().playerPos, GameState::Get().grid);
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
                    deque<POINT> path = aStar.findPath(monster, GameState::Get().playerPos, GameState::Get().grid);
                    pathInfo.emplace_back(path);
                    int id = pathInfo.size() - 1;
                    GameState::Get().monsterPos.insert({ id, monster });
                }
            }
                break;
            case 3:
                {
                    // TODO. 총알 위치 갱신 및 피격 판정
                    using It = list<pair<int, POINT>>::iterator;
                    for (It it = GameState::Get().gun.begin(); it != GameState::Get().gun.end();)
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
                            it = GameState::Get().gun.erase(it);
                            continue;
                        }

                        // 2. 사격 판정
                        bool hit = false;

                        for (int id = 0; id < GameState::Get().monsterPos.size(); ++id)
                        {
                            if (bullet.x == GameState::Get().monsterPos[id].x && bullet.y == GameState::Get().monsterPos[id].y)
                            {
                                hit = true;
                                GameState::Get().monsterPos.erase(id);
                                it = GameState::Get().gun.erase(it);
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

                    if (GameState::Get().waiting) 
                    {
                        GameState::Get().waiting = false;
                        // 게임 종료 
                        GameState::Get().gameOver = true;

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
            POINT next = GameState::Get().playerPos;

            bool a = GetAsyncKeyState(0x41) & 0x8000;
            bool d = GetAsyncKeyState(0x44) & 0x8000;
            bool w = GetAsyncKeyState(0x57) & 0x8000;
            bool s = GetAsyncKeyState(0x53) & 0x8000;

            bool left = GetAsyncKeyState(VK_LEFT) & 0x8000;
            bool right = GetAsyncKeyState(VK_RIGHT) & 0x8000;
            bool up = GetAsyncKeyState(VK_UP) & 0x8000;
            bool down = GetAsyncKeyState(VK_DOWN) & 0x8000;

            if (a && w) 
            {
                next.x -= 1;
                next.y -= 1;
                // 
                GameState::Get().pHoriz = false;
                GameState::Get().pUp = true;
            }
            else if (d && w) 
            {
                next.x += 1;
                next.y -= 1;
                //
                GameState::Get().pHoriz = false;
                GameState::Get().pUp = true;
            }
            else if (a && s) 
            {
                next.x -= 1;
                next.y += 1;
                //
                GameState::Get().pHoriz = false;
                GameState::Get().pUp = false;
            }
            else if (d && s) 
            {
                next.x += 1;
                next.y += 1;
                // 
                GameState::Get().pHoriz = false;
                GameState::Get().pUp = false;
            }
            else if (a) 
            {
                next.x -= 1;
                GameState::Get().pHoriz = true;
                GameState::Get().pFilp = !GameState::Get().pFilp;
            }
            else if (d) 
            {
                next.x += 1;
                GameState::Get().pHoriz = true;
                GameState::Get().pFilp = !GameState::Get().pFilp;
            }
            else if (w) 
            {
                next.y -= 1;
                GameState::Get().pHoriz = false;
                GameState::Get().pUp = true;
            }
            else if (s) 
            {
                next.y += 1;
                GameState::Get().pHoriz = false;
                GameState::Get().pUp = false;
            }

            // 플레이어 충돌 처리
            if (!isInRange(next) || isObstacle(next))
            {
                break;
            }

            // 총알 생성
            ///////////////////////////////////////////////
            int dir = 0;
            POINT bullet = GameState::Get().playerPos;

            if (left && up)
            {
                dir = 0;
                bullet.x -= 1;
                bullet.y -= 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            else if (right && up)
            {
                dir = 1;
                bullet.x += 1;
                bullet.y -= 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            else if (left && down)
            {
                dir = 2;
                bullet.x -= 1;
                bullet.y += 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            else if (right && down)
            {
                dir = 3;
                bullet.x += 1;
                bullet.y += 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            else if (left)
            {
                dir = 4;
                bullet.x -= 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            else if (right)
            {
                dir = 5;
                bullet.x += 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            else if (up)
            {
                dir = 6;
                bullet.y -= 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            else if (down)
            {
                dir = 7;
                bullet.y += 1;

                if (isInRange(bullet) && !isObstacle(bullet))
                {
                    GameState::Get().gun.push_back({ dir, bullet });
                }
            }
            ///////////////////////////////////////////////

            GameState::Get().playerPos = next;

            // 플레이어 이동에 따른 몬스터 경로 갱신
            for (int id = 0; id < GameState::Get().monsterPos.size(); ++id)
            {
                deque<POINT> path = aStar.findPath(GameState::Get().monsterPos[id], GameState::Get().playerPos, GameState::Get().grid);
                pathInfo[id] = path;     
            }

            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        }
        break;
        case WM_PAINT:
        {
            // 앞면 버퍼. 실제 화면
            hdc = BeginPaint(hWnd, &ps);
            // 그리기
            game.render(hdc, hInst);
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

