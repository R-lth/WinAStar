// WinAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI.h"

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
POINT start = { 0,0 };
POINT goal = { 0,0 };
vector<POINT> path;

// 0 : 길, 1 : 벽
vector<vector<int>> grid(20, vector<int>(20, 0));

const int row = static_cast<int>(grid.size());
const int column = static_cast<int>(grid[0].size());
const int cell = 20;

vector<POINT> monsterPos = {};

bool IsSpawnPositionValid(POINT pos) 
{
    if (pos.x < 0 || pos.x >= column || pos.y < 0 || pos.y >= row)
    {
        return false;
    }

    if (grid[pos.y][pos.x])
    {
        return false;
    }

    return true;
}

bool IsPlayerMoveValid(POINT pos) 
{
    if (!IsSpawnPositionValid(pos)) 
    {
        return false;
    }

    for (const POINT& monster : monsterPos)
    {
        if (pos.x == monster.x && pos.y == monster.y)
        {
            return false;
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


// https://m.blog.naver.com/pkk1113/90161680109
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
    HBITMAP Aisle, Brick,Course, Character, Start, Goal;

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
            for (int i = 0; i < 20; ++i)
            {
                grid[0][i] = 1;
                grid[19][i] = 1;
                grid[i][0] = 1;
                grid[i][19] = 1;
            }
         
            for (int i = 7; i < 13; ++i) 
            {
                grid[0][i] = 0;
                grid[19][i] = 0;
                grid[i][0] = 0;
                grid[i][19] = 0;
            }

            // 화면 크기
            width = rect.right - rect.left;
            height = rect.bottom - rect.top;
            
            // 타이머
            SetTimer(hWnd, 1, 3000, NULL);
        }
    break;
    case WM_TIMER:
        {
            const vector<POINT> center = 
            { 
                {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12},
                {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12},
                {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0},
                {7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}
            };
            
            srand(time(NULL));

            int i = rand() % center.size();

            POINT spawn = center[i];
            if (!IsSpawnPositionValid(spawn))
            {
                break;
            }

            monsterPos.push_back(spawn);

            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        }
    break;
    case WM_LBUTTONUP:
    {
        start.x = LOWORD(lParam) / cell;
        start.y = HIWORD(lParam) / cell;

        if (!IsPlayerMoveValid({ start.x, start.y })) 
        {
            break;
        }

        path = aStar.findPath(start, goal, grid);
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
    }
    break;
    case WM_RBUTTONUP:
    {
        goal.x = LOWORD(lParam) / cell;
        goal.y = HIWORD(lParam) / cell;

        if (!IsPlayerMoveValid({ start.x, start.y }))
        {
            break;
        }

        path = aStar.findPath(start, goal, grid);
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
        Character = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER));
        Course = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_COURSE));
        Start = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_START));
        Goal = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOAL));

        // 격자 그리기
        for (int y = 0; y < row; ++y)
        {
            for (int x = 0; x < column; ++x)
            {
                HBITMAP tile = grid[y][x] ? Brick : Aisle;
                SelectObject(scr, tile);
                BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }
        }

        // 경로 그리기
        SelectObject(scr, Course);

        if (!path.empty())
        {
            for (POINT& pos : path)
            {
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }
        }

        // 시작 위치 표시
        SelectObject(scr, Start);
        BitBlt(back, start.x * cell, start.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
        // 캐릭터 표시
        SelectObject(scr, Goal);
        BitBlt(back, goal.x * cell, goal.y * cell, cell, cell, scr, 0, 0, SRCCOPY);

        // TODO. 플레이어 크기를 40*40으로 변경 후, 위치 지정 및 이동 고려하기
        // TODO. 장애물 이동
        // 장애물 그리기
        SelectObject(scr, Character);
        for (const POINT& pos : monsterPos) 
        {
            BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
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
        DeleteObject(Course);
        DeleteObject(Character);
        DeleteObject(Start);
        DeleteObject(Goal);
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
            KillTimer(hWnd, 1);
            PostQuitMessage(0);
        }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

