#define _WIN32_WINNT 0x0601
#include <windows.h>
#include "common.h"
#include "button.h"
#include "menu.h"
#include "reaction.h"
#include "setting.h"
#include "options.h"
#include "camera.h"
#include "threeballs.h"
#include "follow.h"
#include <ctime>

bool running = true;
bool is_game_started;
bool is_options_open = false;
bool is_reaction = false;
bool is_threeballs = false;
bool is_follow = false;
GameMode game_mode = GameMode::reaction;
Setting setting;
Camera camera;
int mouse_dx = 0;
int mouse_dy = 0;
bool cursor_locked = false;

WNDPROC old_wndproc;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_SETCURSOR && cursor_locked)
    {
        SetCursor(NULL);
        return TRUE;
    }
    return CallWindowProc(old_wndproc, hwnd, msg, wParam, lParam);
}

int main()
{
    setting.load();

    srand((unsigned)time(0));

    initgraph(window_width, window_height);
    old_wndproc = (WNDPROC)GetWindowLongPtr(GetHWnd(), GWLP_WNDPROC);
    SetWindowLongPtr(GetHWnd(), GWLP_WNDPROC, (LONG_PTR)WndProc);

    InitMenu();
    InitReaction();
    initoptions();
    initthreeballs();
    initfollow();

    int screen_cx = window_width / 2;
    int screen_cy = window_height / 2;

    BeginBatchDraw();
    DWORD last_loop_time = GetTickCount();
    while (running)
    {
        DWORD loop_now = GetTickCount();
        float dt = (loop_now - last_loop_time) / 1000.0f;
        last_loop_time = loop_now;

        bool in_game = (is_game_started && (game_mode == GameMode::threeballs || game_mode == GameMode::follow));

        cursor_locked = in_game;

        if (in_game)
        {
            HWND hwnd = GetHWnd();
            POINT center = {screen_cx, screen_cy};
            ClientToScreen(hwnd, &center);
            POINT pt;
            GetCursorPos(&pt);
            mouse_dx = pt.x - center.x;
            mouse_dy = pt.y - center.y;
            if (mouse_dx != 0 || mouse_dy != 0)
                SetCursorPos(center.x, center.y);
        }

        ExMessage msg;
        while (peekmessage(&msg))
        {
            if (is_game_started && game_mode == GameMode::reaction)
            {
                processreactionevent(msg);
            }
            else if (is_options_open)
            {
                processoptionsevent(msg);
            }
            else if (is_game_started && game_mode == GameMode::threeballs)
            {
                processthreeballs(msg);
            }
            else if (is_game_started && game_mode == GameMode::follow)
            {
                followprocess(msg);
            }
            else
            {
                ProcessMenuEvent(msg);
            }
        }

        DWORD start_time = GetTickCount();

        float sens = setting.getsen();
        camera.x += mouse_dx * sens;
        camera.y += mouse_dy * sens;
        mouse_dx = 0;
        mouse_dy = 0;

        cleardevice();

        if (is_game_started)
        {
            if (game_mode == GameMode::reaction)
            {
                updatereaction();
                DrawReaction();
            }
            else if (game_mode == GameMode::threeballs)
            {
                drawthreeballs();
            }
            else if (game_mode == GameMode::follow)
            {
                updatefollow(dt);
                drawfollow();
            }
        }
        else if (is_options_open)
        {
            drawoptions();
        }
        else
        {
            DrawMenu();
        }

        FlushBatchDraw();

        DWORD end_time = GetTickCount();
        DWORD delta_time = end_time - start_time;
        if (delta_time < 1000 / 180)
        {
            Sleep(1000 / 180 - delta_time);
        }
    }
    EndBatchDraw();

    return 0;
}
