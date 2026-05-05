#include "common.h"
#include "button.h"
#include "menu.h"
#include "reaction.h"

bool running = true;
bool is_game_started;
bool is_options_open = false;
bool is_reaction = false;
bool is_threeballs = false;
bool is_follow = false;
GameMode game_mode = GameMode::reaction;

int main()
{
    initgraph(window_width, window_height);
    InitMenu();
    InitReaction();

    BeginBatchDraw();
    while (running)
    {
        ExMessage msg;
        while (peekmessage(&msg))
        {
            if (is_game_started && game_mode == GameMode::reaction)
            {
                processreactionevent(msg);
            }
            else if (is_options_open)
            {
                // 选项菜单后续需另外续写
            }
            else
            {
                ProcessMenuEvent(msg);
            }
        }
        DWORD start_time = GetTickCount();

        cleardevice();

        if (is_game_started)
        {
            if (game_mode == GameMode::reaction)
            {
                updatereaction();
                DrawReaction();
            }
        }
        else if (is_options_open)
        {
            // 选项菜单绘制后续需另外续写
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
