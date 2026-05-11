#pragma once
#include <graphics.h>
#include "common.h"

void initthreeballs();
void initfollow();

class Button
{
public:
    Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
    {
        region = rect;

        loadimage(&img_idle, path_img_idle);
        loadimage(&img_hovered, path_img_hovered);
        loadimage(&img_pushed, path_img_pushed);
    }

    ~Button() = default;

    void Draw()
    {
        switch (status)
        {
        case Status::idle:
            putimage(region.left, region.top, &img_idle);
            break;
        case Status::hovered:
            putimage(region.left, region.top, &img_hovered);
            break;
        case Status::pushed:
            putimage(region.left, region.top, &img_pushed);
            break;
        }
    }

    void processevent(const ExMessage& msg)
    {
        switch (msg.message)
        {
        case WM_MOUSEMOVE:
            if (status == Status::idle && checkcursorhit(msg.x, msg.y))
                status = Status::hovered;
            else if (status == Status::hovered && !checkcursorhit(msg.x, msg.y))
                status = Status::idle;
            break;
        case WM_LBUTTONDOWN:
            if (checkcursorhit(msg.x, msg.y))
                status = Status::pushed;
            break;
        case WM_LBUTTONUP:
            if (status == Status::pushed)
            {
                onclick();
                status = Status::idle;
            }
            break;
        default:
            break;
        }
    }

protected:
    virtual void onclick() = 0;

private:
    enum class Status
    {
        idle = 0,
        hovered,
        pushed
    };

private:
    RECT region;
    IMAGE img_idle;
    IMAGE img_hovered;
    IMAGE img_pushed;
    Status status = Status::idle;

private:
    bool checkcursorhit(int x, int y)
    {
        return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
    }
};

class threeballsbutton : public Button
{
public:
    threeballsbutton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed) : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~threeballsbutton() = default;

protected:
    void onclick()
    {
        initthreeballs();
        is_game_started = true;
        game_mode = GameMode::threeballs;
    }
};

class followbutton : public Button
{
public:
    followbutton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed) : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~followbutton() = default;

protected:
    void onclick()
    {
        initfollow();
        is_game_started = true;
        game_mode = GameMode::follow;
    }
};

class reactionbutton : public Button
{
public:
    reactionbutton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed) : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~reactionbutton() = default;

protected:
    void onclick()
    {
        is_game_started = true;
        game_mode = GameMode::reaction;
    }
};

class optionsbutton : public Button
{
public:
    optionsbutton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed) : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~optionsbutton() = default;

protected:
    void onclick()
    {
        is_options_open = true;
    }
};

class quitbutton : public Button
{
public:
    quitbutton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed) : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~quitbutton() = default;

protected:
    void onclick()
    {
        running = false;
    }
};
