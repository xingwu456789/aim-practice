#include "follow.h"
#include <tchar.h>
#include <cstdio>
#include <cmath>

Ball_follow ball_follow;

enum FollowState
{
    FOLLOW_START,
    FOLLOW_PLAYING,
    FOLLOW_ENDING
};

static FollowState state;
static IMAGE img_start, img_ending;

static float score;
static int frames_on_target;
static int total_frames;
static float longest_streak;
static float current_streak;
static DWORD follow_start_time;

static float random_range(float min, float max)
{
    return min + (float)rand() / RAND_MAX * (max - min);
}

static void pick_new_target()
{
    const float max_range_x = 500;
    const float max_range_y = 350;
    const float min_dist = 150;

    for (int attempt = 0; attempt < 50; ++attempt)
    {
        float tx = camera.x + random_range(-max_range_x, max_range_x);
        float ty = camera.y + random_range(-max_range_y, max_range_y);

        float dx = tx - ball_follow.world_x;
        float dy = ty - ball_follow.world_y;
        if (dx * dx + dy * dy >= min_dist * min_dist)
        {
            ball_follow.target_x = tx;
            ball_follow.target_y = ty;
            return;
        }
    }

    ball_follow.target_x = ball_follow.world_x + random_range(-300, 300);
    ball_follow.target_y = ball_follow.world_y + random_range(-200, 200);
}

void initfollow()
{
    score = 0;
    frames_on_target = 0;
    total_frames = 0;
    longest_streak = 0;
    current_streak = 0;

    ball_follow.world_x = camera.x;
    ball_follow.world_y = camera.y;
    ball_follow.wait_timer = 0;
    pick_new_target();

    state = FOLLOW_START;
    loadimage(&img_start, _T("images/follow_start.png"));
    loadimage(&img_ending, _T("images/follow_ending.png"));
}

void updatefollow(float dt)
{
    if (state != FOLLOW_PLAYING)
        return;

    DWORD elapsed = GetTickCount() - follow_start_time;
    if (elapsed >= 60000)
    {
        state = FOLLOW_ENDING;
        return;
    }

    if (ball_follow.wait_timer > 0)
    {
        ball_follow.wait_timer -= dt;
    }
    else
    {
        float dx = ball_follow.target_x - ball_follow.world_x;
        float dy = ball_follow.target_y - ball_follow.world_y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < ball_follow.speed * dt)
        {
            ball_follow.world_x = ball_follow.target_x;
            ball_follow.world_y = ball_follow.target_y;
            ball_follow.wait_timer = random_range(0.3f, 1.5f);
            pick_new_target();
        }
        else
        {
            ball_follow.world_x += dx / dist * ball_follow.speed * dt;
            ball_follow.world_y += dy / dist * ball_follow.speed * dt;
        }
    }

    float cx = camera.x - ball_follow.world_x;
    float cy = camera.y - ball_follow.world_y;
    float dist_to_ball = sqrtf(cx * cx + cy * cy);

    total_frames++;

    if (dist_to_ball < ball_follow.radius)
    {
        float precision = 1.0f - (dist_to_ball / ball_follow.radius);
        score += precision * dt * 1000;
        frames_on_target++;
        current_streak += dt;
        if (current_streak > longest_streak)
            longest_streak = current_streak;
    }
    else
    {
        current_streak = 0;
    }
}

void followprocess(const ExMessage &msg)
{
    switch (state)
    {
    case FOLLOW_START:
        if (msg.message == WM_LBUTTONDOWN)
        {
            state = FOLLOW_PLAYING;
            follow_start_time = GetTickCount();
        }
        break;
    case FOLLOW_PLAYING:
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
        {
            state = FOLLOW_START;
            is_game_started = false;
        }
        break;
    case FOLLOW_ENDING:
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
            is_game_started = false;
        break;
    }
}

void drawfollow()
{
    int center_x = 960, center_y = 540;

    switch (state)
    {
    case FOLLOW_START:
        putimage(0, 0, &img_start);
        break;
    case FOLLOW_PLAYING:
    {
        int sx = (int)(ball_follow.world_x - camera.x + center_x);
        int sy = (int)(ball_follow.world_y - camera.y + center_y);

        float dx = camera.x - ball_follow.world_x;
        float dy = camera.y - ball_follow.world_y;
        bool on_target = (dx * dx + dy * dy < ball_follow.radius * ball_follow.radius);

        setfillcolor(on_target ? RGB(80, 255, 80) : RGB(255, 255, 255));
        fillcircle(sx, sy, (int)ball_follow.radius);

        setlinecolor(RGB(255, 80, 80));
        line(center_x - 8, center_y, center_x + 8, center_y);
        line(center_x, center_y - 8, center_x, center_y + 8);

        float accuracy = total_frames > 0 ? (float)frames_on_target / total_frames * 100 : 0;
        int remaining = 60 - (int)((GetTickCount() - follow_start_time) / 1000);
        if (remaining < 0)
            remaining = 0;

        setbkmode(TRANSPARENT);
        settextstyle(30, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));

        TCHAR buf[64];
        _stprintf(buf, _T("得分: %.0f"), score);
        outtextxy(10, 10, buf);

        _stprintf(buf, _T("准确率: %.1f%%"), accuracy);
        outtextxy(10, 50, buf);

        _stprintf(buf, _T("最长跟枪时间: %.1fs"), longest_streak);
        outtextxy(10, 90, buf);

        _stprintf(buf, _T("剩余时间: %ds"), remaining);
        outtextxy(1600, 10, buf);

        break;
    }
    case FOLLOW_ENDING:
    {
        putimage(0, 0, &img_ending);

        float accuracy = total_frames > 0 ? (float)frames_on_target / total_frames * 100 : 0;

        setbkmode(TRANSPARENT);
        settextstyle(100, 0, _T("微软雅黑"));
        settextcolor(RGB(255, 255, 255));

        TCHAR buf[32];
        _stprintf(buf, _T("%.0f"), score);
        outtextxy(880, 256, buf);

        _stprintf(buf, _T("%.1f%%"), accuracy);
        outtextxy(880, 408, buf);

        _stprintf(buf, _T("%.1fs"), longest_streak);
        outtextxy(880, 560, buf);

        break;
    }
    }
}
