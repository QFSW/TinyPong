#include <cstdint>
#include <windows.h>

#include <paddle.hpp>
#include <key_map.hpp>
#include <game.hpp>

constexpr uint8_t player_count = 2;

Paddle players[player_count];
Vector2 size = { 100, 100 };
KeyMap key_map = {};

void set_vel()
{
    players[0].vel.y = key_map.up - key_map.down;
    players[0].vel.x = key_map.right - key_map.left;
    players[1].vel.y = key_map.w - key_map.s;
    players[1].vel.x = key_map.d - key_map.a;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void debug(int val)
{
    constexpr uint8_t buf_size = 20;
    char buf[buf_size] = {};
    char msg[buf_size] = {};

    uint8_t i = 0;
    for (; i < buf_size; ++i)
    {
        if (val <= 0) break;

        buf[i] = '0' + val % 10;
        val /= 10;
    }

    for (uint8_t j = 0; j < i; ++j)
    {
        msg[j] = buf[i - j - 1];
    }
    msg[i] = 0;

    MessageBoxA(nullptr, msg, "Debug", MB_OK);
}

int WINAPI main()
{
    wchar_t* lpszCommandLine = GetCommandLineW();

    // skip past program name (first token in command line).
    if (*lpszCommandLine == '"')  // check for and handle quoted program name
    {
        // scan, and skip over, subsequent characters until  another
        // double-quote or a null is encountered
        while (*lpszCommandLine && (*lpszCommandLine != '"'))
            lpszCommandLine++;

        // if we stopped on a double-quote (usual case), skip over it.
        if (*lpszCommandLine == '"')
            lpszCommandLine++;
    }
    else
    {
        // first token wasn't a quote
        while (*lpszCommandLine > ' ')
            lpszCommandLine++;
    }

    // skip past any white space preceeding the second token.
    while (*lpszCommandLine && (*lpszCommandLine <= ' '))
        lpszCommandLine++;

    STARTUPINFO	startup_info = { sizeof(STARTUPINFO),0 };
    // GetStartupInfo(&startup_info);

    return wWinMain(
        GetModuleHandle(nullptr),
        nullptr,
        lpszCommandLine,
        startup_info.dwFlags & STARTF_USESHOWWINDOW
            ? startup_info.wShowWindow
            : SW_SHOWDEFAULT
    );
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const char* window_name = "Tiny Pong";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = window_name;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        window_name,                    // Window class
        window_name,                    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    Game game(hwnd, players);
    MSG msg = { };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            game.tick();
        }
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_UP: key_map.up = true; break;
                case VK_DOWN: key_map.down = true; break;
                case VK_LEFT: key_map.left = true; break;
                case VK_RIGHT: key_map.right = true; break;
                case 'W': key_map.w = true; break;
                case 'S': key_map.s = true; break;
                case 'A': key_map.a = true; break;
                case 'D': key_map.d = true; break;
            }

            set_vel();
            return 0;
        }

        case WM_KEYUP:
        {
            switch (wParam)
            {
                case VK_UP: key_map.up = false; break;
                case VK_DOWN: key_map.down = false; break;
                case VK_LEFT: key_map.left = false; break;
                case VK_RIGHT: key_map.right = false; break;
                case 'W': key_map.w = false; break;
                case 'S': key_map.s = false; break;
                case 'A': key_map.a = false; break;
                case 'D': key_map.d = false; break;
            }

            set_vel();
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT win_rect = {};

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 3));

            GetClientRect(hwnd, &win_rect);
            const int width = win_rect.right - win_rect.left;
            const int height = win_rect.bottom - win_rect.top;

            for (uint8_t i = 0; i < player_count; ++i)
            {
                RECT p = {
                    .left = width / 2 - size.x / 2 + players[i].pos.x,
                    .top = height / 2 - size.y / 2 - players[i].pos.y,
                    .right = width / 2 + size.x / 2 + players[i].pos.x,
                    .bottom = height / 2 + size.x / 2 - players[i].pos.y,
                };

                FillRect(hdc, &p, (HBRUSH)(COLOR_WINDOW + 1));
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
