# Step-by-Step: Build Snake Game (WinAPI) 

1. Open Visual Studio 2022
- Click "Create a new project".

2. Choose Project Template
- Search for: "Windows Desktop Application" (C++).
- Select it and click Next.

3. Configure the Project
- Name: SnakeWinAPI
- Location: Choose any folder
- Project Type: Windows Application (.exe)
- Click Create

4. Replace the Default Code
- Open SnakeWinAPI.cpp (or similar).
- Replace the contents with this code:

```bash
#include <windows.h>
#include <vector>
#include <ctime>

const int TILE_SIZE = 20;
const int WIDTH = 800;
const int HEIGHT = 600;
const int COLUMNS = WIDTH / TILE_SIZE;
const int ROWS = HEIGHT / TILE_SIZE;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
};

std::vector<Point> snake;
Point food;
Direction dir = RIGHT;
bool gameOver = false;

void InitGame() {
    snake.clear();
    snake.push_back({ COLUMNS / 2, ROWS / 2 });
    food = { rand() % COLUMNS, rand() % ROWS };
    dir = RIGHT;
}

void MoveSnake() {
    Point head = snake.front();
    switch (dir) {
    case UP: head.y--; break;
    case DOWN: head.y++; break;
    case LEFT: head.x--; break;
    case RIGHT: head.x++; break;
    }

    if (head.x < 0 || head.x >= COLUMNS || head.y < 0 || head.y >= ROWS)
        gameOver = true;

    for (auto& s : snake)
        if (s.x == head.x && s.y == head.y)
            gameOver = true;

    snake.insert(snake.begin(), head);

    if (head.x == food.x && head.y == food.y) {
        food = { rand() % COLUMNS, rand() % ROWS };
    }
    else {
        snake.pop_back();
    }
}

void Draw(HDC hdc) {
    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 200, 0));
    HBRUSH redBrush = CreateSolidBrush(RGB(200, 0, 0));

    for (auto& s : snake) {
        RECT r = { s.x * TILE_SIZE, s.y * TILE_SIZE,
                   (s.x + 1) * TILE_SIZE, (s.y + 1) * TILE_SIZE };
        FillRect(hdc, &r, greenBrush);
    }

    RECT fr = { food.x * TILE_SIZE, food.y * TILE_SIZE,
                (food.x + 1) * TILE_SIZE, (food.y + 1) * TILE_SIZE };
    FillRect(hdc, &fr, redBrush);

    DeleteObject(greenBrush);
    DeleteObject(redBrush);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        SetTimer(hwnd, 1, 100, NULL);
        break;
    case WM_TIMER:
        if (!gameOver) {
            MoveSnake();
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_UP:    if (dir != DOWN)  dir = UP; break;
        case VK_DOWN:  if (dir != UP)    dir = DOWN; break;
        case VK_LEFT:  if (dir != RIGHT) dir = LEFT; break;
        case VK_RIGHT: if (dir != LEFT)  dir = RIGHT; break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Draw(hdc);
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    srand((unsigned)time(NULL));
    InitGame();

    const wchar_t CLASS_NAME[] = L"SnakeWinClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Snake Game (WinAPI)", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH + 16, HEIGHT + 39,
        NULL, NULL, hInst, NULL
    );


    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

```
5. Build & Run
- Press Ctrl + F5 to build and run.

Use Arrow keys to play!
