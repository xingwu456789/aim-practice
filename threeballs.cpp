#include "threeballs.h"
#include <tchar.h>
#include <cstdio>

static DWORD threeballs_start_time = 0;
static DWORD threeballs_continue_time = 0;
Ball balls[3];
static int score = 0;
static int counter = 0;
static int KPM = 0;
float hit_rate = 0;
DWORD now_time = 0;

enum threeballsstate
{
    threeballsstart,
    threeballsprocess,
    threeballsending
};

static threeballsstate state;

IMAGE threeballs_start;
IMAGE threeballs_ending;

void spawnball(int index)
{
    const float min_dis = balls[0].radius * 2 + 10;

    for (int attempt = 0; attempt < 100; ++attempt)
    {
        float wx = 960 + (rand() % 800 - 400);
        float wy = 540 + (rand() % 400 - 200);

        bool overlap = false;
        for (int j = 0; j < 3; ++j)
        {
            if (j == index)
                continue;
            float dx = wx - balls[j].world_x;
            float dy = wy - balls[j].world_y;
            if (dx * dx + dy * dy < min_dis * min_dis)
            {
                overlap = true;
                break;
            }
        }

        if (!overlap)
        {
            balls[index].world_x = wx;
            balls[index].world_y = wy;
            return;
        }
    }
}

void initthreeballs()
{
    score = 0;
    counter = 0;
    hit_rate = 0;
    KPM = 0;
    threeballs_start_time = 0;
    state = threeballsstart;
    for (int i = 0; i < 3; ++i)
    {
        spawnball(i);
    }
    loadimage(&threeballs_start, _T("images/threeballs_start.png"));
    loadimage(&threeballs_ending, _T("images/threeballs_ending.png"));
}

void processthreeballs(const ExMessage &msg)
{
    switch (state)
    {
    case threeballsstart:
        if (msg.message == WM_LBUTTONDOWN)
        {
            state = threeballsprocess;
            threeballs_start_time = GetTickCount();
            break;
        }
        break;
    case threeballsprocess:
    {
        if (msg.message != WM_LBUTTONDOWN && !(msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE))
            return;
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
        {
            state = threeballsstart;
            is_game_started = false;
            return;
        }

        threeballs_continue_time = GetTickCount();
        if ((threeballs_continue_time - threeballs_start_time) / 1000 >= 60)
        {
            state = threeballsending;
            return;
        }

        ++counter;
        for (int i = 0; i < 3; i++)
        {
            float dx = camera.x - balls[i].world_x;
            float dy = camera.y - balls[i].world_y;
            if (dx * dx + dy * dy <= balls[i].radius * balls[i].radius)
            {
                ++score;
                spawnball(i);
                break;
            }
        }
        break;
    }
    case threeballsending:
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
        {
            is_game_started = false;
            return;
        }
        break;
    }
}

void drawthreeballs()
{
    switch (state)
    {
    case threeballsstart:
        putimage(0, 0, &threeballs_start);
        break;
    case threeballsprocess:
    {
        now_time = GetTickCount() - threeballs_start_time;
        if (now_time > 1000)
        {
            KPM = score * 60 / (now_time / 1000);
        }
        else
        {
            KPM = 0;
        }

        if (counter > 0)
        {
            hit_rate = (float)score / (float)counter;
        }
        else
        {
            hit_rate = 0.0f;
        }
        for (int i = 0; i < 3; ++i)
        {
            int sx = (int)(balls[i].world_x - camera.x + 960);
            int sy = (int)(balls[i].world_y - camera.y + 540);
            fillcircle(sx, sy, balls[i].radius);
        }
        int center_x = 960, center_y = 540;
        setlinecolor(RGB(255, 80, 80));
        line(center_x - 5, center_y, center_x + 5, center_y);
        line(center_x, center_y - 5, center_x, center_y + 5);

        setbkmode(TRANSPARENT);
        settextstyle(40, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));
        {
            TCHAR buf[32];
            wsprintf(buf, _T("KPM: %d"), KPM);
            outtextxy(10, 10, buf);
        }

        setbkmode(TRANSPARENT);
        settextstyle(40, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));
        {
            TCHAR buf1[32];
            _stprintf(buf1, _T("命中率: %.2f%%"), hit_rate * 100);
            outtextxy(1600, 10, buf1);
        }
        break;
    }
    case threeballsending:
        putimage(0, 0, &threeballs_ending);

        setbkmode(TRANSPARENT);
        settextstyle(100, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));
        {
            TCHAR buf[32];
            wsprintf(buf, _T("%d"), KPM);
            outtextxy(880, 206, buf);
        }

        setbkmode(TRANSPARENT);
        settextstyle(100, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));
        {
            TCHAR buf1[32];
            _stprintf(buf1, _T("%.2f%%"), hit_rate * 100);
            outtextxy(880, 458, buf1);
        }
        break;
    }
}