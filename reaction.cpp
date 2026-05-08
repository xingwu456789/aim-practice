#include "common.h"
#include <tchar.h>
enum Reactionstate
{
    start,
    wait,
    click,
    toosoon,
    result,
    ending
};

static int trial_count = 0;
static int reaction_times[5] = {0};

double wait_end_time;
double reaction_start_time;
static Reactionstate state;

static IMAGE reaction_start;
static IMAGE reaction_wait;
static IMAGE reaction_click;
static IMAGE reaction_toosoon;
static IMAGE reaction_continue;
static IMAGE reaction_ending;

static LARGE_INTEGER s_counter;
static double gettime()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart * 1000.0 / s_counter.QuadPart;
}
void InitReaction()
{
    loadimage(&reaction_start, _T("images/reaction_start.png"));
    loadimage(&reaction_wait, _T("images/reaction_wait.png"));
    loadimage(&reaction_click, _T("images/reaction_click.png"));
    loadimage(&reaction_toosoon, _T("images/reaction_toosoon.png"));
    loadimage(&reaction_continue, _T("images/reaction_continue.png"));
    loadimage(&reaction_ending, _T("images/reaction_ending.png"));
    QueryPerformanceFrequency(&s_counter);
}

void processreactionevent(ExMessage &msg)
{
    switch (state)
    {
    case start:
        if (msg.message == WM_LBUTTONDOWN)
        {
            wait_end_time = gettime() + rand() % 3000 + 1000;
            state = wait;
            break;
        }
        break;
    case wait:
        if (msg.message == WM_LBUTTONDOWN)
        {
            state = toosoon;
            break;
        }
        break;
    case click:
        if (msg.message == WM_LBUTTONDOWN)
        {
            reaction_times[trial_count] = gettime() - reaction_start_time;
            ++trial_count;
            state = result;
            if (trial_count == 5)
            {
                if (msg.message == WM_LBUTTONDOWN)
                {
                    state = ending;
                    break;
                }
            }
            break;
        }
        break;
    case toosoon:
        if (msg.message == WM_LBUTTONDOWN)
        {
            wait_end_time = gettime() + rand() % 3000 + 1000;
            state = wait;
        }
        break;
    case result:
        if (msg.message == WM_LBUTTONDOWN)
        {
            wait_end_time = gettime() + rand() % 3000 + 1000;
            state = wait;
        }
        break;
    case ending:
        if (msg.message == WM_LBUTTONDOWN)
        {
            is_game_started = false;
            trial_count = 0;
            state = start;
        }
        break;
    }
}
void updatereaction()
{
    if (state == wait && gettime() >= wait_end_time)
    {
        reaction_start_time = gettime();
        state = click;
    }
}
void DrawReaction()
{
    switch (state)
    {
    case start:
        putimage(0, 0, &reaction_start);
        break;
    case wait:
        putimage(0, 0, &reaction_wait);
        break;
    case click:
        putimage(0, 0, &reaction_click);
        break;
    case toosoon:
        putimage(0, 0, &reaction_toosoon);
        break;
    case result:
        putimage(0, 0, &reaction_continue);
        setbkmode(TRANSPARENT);
        settextstyle(200, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));
        {
            TCHAR buf[32];
            wsprintf(buf, _T("%d ms"), reaction_times[trial_count - 1]);
            outtextxy(760, 380, buf);
        }
        break;
    case ending:
        putimage(0, 0, &reaction_ending);
        setbkmode(TRANSPARENT);
        settextstyle(200, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));
        {
            double sum = 0;
            for (int i = 0; i < 5; ++i)
                sum += reaction_times[i];
            int ave = (int)(sum / 5);
            TCHAR buf[32];
            wsprintf(buf, _T("%d ms"), ave);
            outtextxy(760, 380, buf);
        }
        break;
    }
}
