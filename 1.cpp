#include <graphics.h>

bool running = true;
bool is_game_started = false;
bool is_options_open = false;

enum class GameMode
{
    reaction,
    threeballs,
    follow,
    hold
};
GameMode game_mode = GameMode::reaction;

const int window_width = 1920;
const int window_height = 720;

const int button_width = 200;
const int button_height = 91;

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

    void processevent(const ExMessage &msg)
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
                onclick();
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
        is_game_started = true;
        game_mode = GameMode::follow;
    }
};

class holdbutton : public Button
{
public:
    holdbutton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed) : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~holdbutton() = default;

protected:
    void onclick()
    {
        is_game_started = true;
        game_mode = GameMode::hold;
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

int main()
{
    initgraph(1920, 1080);

    IMAGE menu_background;
    loadimage(&menu_background, _T("images/menu_background.png"));

    RECT region_menu_reaction, region_menu_quit;

    region_menu_reaction.left = 710;
    region_menu_reaction.right = 910;
    region_menu_reaction.top = 600;
    region_menu_reaction.bottom = 691;

    region_menu_quit.left = 1010;
    region_menu_quit.right = 1210;
    region_menu_quit.top = 800;
    region_menu_quit.bottom = 891;

    reactionbutton menu_reaction = reactionbutton(region_menu_reaction, _T("images/menu_reaction_idle.png"), _T("images/menu_reaction_hovered.png"), _T("images/menu_reaction_pushed.png"));
    quitbutton menu_quit = quitbutton(region_menu_quit, _T("images/menu_quit_idle.png"), _T("images/menu_quit_hovered.png"), _T("images/menu_quit_pushed.png"));

    BeginBatchDraw();
    while (running)
    {
        ExMessage msg;
        while (peekmessage(&msg))
        {
            if (is_game_started)
            { // gamestart后续需另外续写
            }
            else
            {
                menu_reaction.processevent(msg);
                menu_quit.processevent(msg);
            }
        }
        DWORD start_time = GetTickCount();

        cleardevice();

        if (is_game_started)
        {
        }
        else
        {
            putimage(0, 0, &menu_background);
            menu_reaction.Draw();
            menu_quit.Draw();
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