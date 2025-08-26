// WinAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI.h"

#include "pch.h"
#include "AStar.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;                                      // 창 핸들 전역으로 초기화.

#pragma region 전역
AStar aStar;
POINT start = { 0,0 };
POINT goal = { 0,0 };
vector<POINT> path;

// 0 : 길, 1 : 벽
vector<vector<int>> grid =
{
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }  
};

const int row = static_cast<int>(grid.size());
const int column = static_cast<int>(grid[0].size());
const int cell = 20;
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
   RECT windowRect = { 0, 0, GWinSizeX, GWinSizeY }; // 800, 600
   ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
    // TODO.
    case WM_CREATE:
        break;
    case WM_TIMER:
        break;
    case WM_LBUTTONUP:
        {
            POINT goal;
            goal.x = HIWORD(lParam) / cell;
            goal.y = LOWORD(lParam) / cell;

            if (goal.x < 0 || goal.x >= column || goal.y < 0 || goal.y >= row)
            {
                break;
            }

            if (grid[goal.y][goal.x] == 1)
            {
                break;
            }

            path = aStar.findPath(start, goal, grid);
            // 창 업데이트
            RedrawWindow(hWnd, NULL, NULL, 1);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
#pragma region 초기화
            // 메모리 DC, 버블 버퍼링
            HDC back = CreateCompatibleDC(hdc); // 스케치북
            HDC scr = CreateCompatibleDC(hdc);  // 제출할 최종 스케치북
            // 1. 컬러 비트맵
            // https://learn.microsoft.com/ko-kr/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap
            HBITMAP bmp = CreateCompatibleBitmap(hdc, GWinSizeX, GWinSizeY); // 비트맵 전체 너비, 높이
            // TODO 2. 
            // https://soonang2.tistory.com/29
            HBITMAP oldBmp = (HBITMAP)SelectObject(back, bmp);
            // 3. 비트맵
            HBITMAP Aisle = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AISLE));
            HBITMAP Wall = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WALL));
            // TODO. 이름 변경하기... path와 같은 맥락 
            HBITMAP Road = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROAD));
            HBITMAP Goal = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOAL));
#pragma endregion

#pragma region 그리기
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            //game.Render();
#pragma endregion

#pragma region 해제
            DeleteObject(Aisle);
            DeleteObject(Wall);
            DeleteObject(Road);
            DeleteObject(Goal);
            DeleteObject(bmp);
            DeleteObject(oldBmp);
            DeleteDC(back);
            DeleteDC(scr);
#pragma endregion
            // 제출
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
