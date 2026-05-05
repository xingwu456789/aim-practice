#include "menu.h"
#include "button.h"
#include "common.h"

static IMAGE menu_background;

static reactionbutton *menu_reaction;
static threeballsbutton *menu_threeballs;
static followbutton *menu_follow;
static optionsbutton *menu_options;
static quitbutton *menu_quit;

void InitMenu()
{
    loadimage(&menu_background, _T("images/menu_background.png"));

    RECT region_menu_reaction, region_menu_threeballs, region_menu_follow, region_menu_options, region_menu_quit;

    region_menu_threeballs.left = 710;
    region_menu_threeballs.right = 910;
    region_menu_threeballs.top = 500;
    region_menu_threeballs.bottom = 591;

    region_menu_follow.left = 1010;
    region_menu_follow.right = 1210;
    region_menu_follow.top = 500;
    region_menu_follow.bottom = 591;

    region_menu_reaction.left = 860;
    region_menu_reaction.right = 1060;
    region_menu_reaction.top = 620;
    region_menu_reaction.bottom = 711;

    region_menu_options.left = 710;
    region_menu_options.right = 910;
    region_menu_options.top = 740;
    region_menu_options.bottom = 831;

    region_menu_quit.left = 1010;
    region_menu_quit.right = 1210;
    region_menu_quit.top = 740;
    region_menu_quit.bottom = 831;

    menu_reaction = new reactionbutton(region_menu_reaction, _T("images/menu_reaction_idle.png"), _T("images/menu_reaction_hovered.png"), _T("images/menu_reaction_pushed.png"));
    menu_threeballs = new threeballsbutton(region_menu_threeballs, _T("images/menu_threeballs_idle.png"), _T("images/menu_threeballs_hovered.png"), _T("images/menu_threeballs_pushed.png"));
    menu_follow = new followbutton(region_menu_follow, _T("images/menu_follow_idle.png"), _T("images/menu_follow_hovered.png"), _T("images/menu_follow_pushed.png"));
    menu_options = new optionsbutton(region_menu_options, _T("images/menu_options_idle.png"), _T("images/menu_options_hovered.png"), _T("images/menu_options_pushed.png"));
    menu_quit = new quitbutton(region_menu_quit, _T("images/menu_quit_idle.png"), _T("images/menu_quit_hovered.png"), _T("images/menu_quit_pushed.png"));
}

void ProcessMenuEvent(const ExMessage &msg)
{
    menu_reaction->processevent(msg);
    menu_threeballs->processevent(msg);
    menu_follow->processevent(msg);
    menu_options->processevent(msg);
    menu_quit->processevent(msg);
}

void DrawMenu()
{
    putimage(0, 0, &menu_background);
    menu_reaction->Draw();
    menu_threeballs->Draw();
    menu_follow->Draw();
    menu_options->Draw();
    menu_quit->Draw();
}
