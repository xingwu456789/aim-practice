#include "options.h"
#include <tchar.h>
#include <cstdio>

static int selected;

void initoptions()
{
    selected = 0;
}
void processoptionsevent(const ExMessage &msg)
{
    if (msg.message == WM_KEYDOWN)
    {
        switch (msg.vkcode)
        {
        case VK_UP:
            selected = (selected - 1 + 2) % 2;
            break;
        case VK_DOWN:
            selected = (selected + 1) % 2;
            break;
        case VK_RIGHT:
            if (selected == 0)
            {
                setting.gamekind = (setting.gamekind + 1) % 2;
                break;
            }
            else if (selected == 1)
            {
                if (setting.sensitivity >= 6.0)
                    break;
                else
                {
                    setting.sensitivity += 0.01;
                    break;
                }
            }
            // else
            // {
            //     if (setting.volumn < 100)
            //     {
            //         ++setting.volumn;
            //         break;
            //     }
            //     else
            //         break;
            // }
        case VK_LEFT:
            if (selected == 0)
            {
                setting.gamekind = (setting.gamekind - 1) % 2;
                break;
            }
            else if (selected == 1)
            {
                if (setting.sensitivity <= 0.01)
                    break;
                else
                {
                    setting.sensitivity -= 0.01;
                    break;
                }
            }
            // else
            // {
            //     if (setting.volumn > 0)
            //     {
            //         --setting.volumn;
            //         break;
            //     }
            //     else
            //         break;
            // }
        case VK_ESCAPE:
            setting.save();
            is_options_open = false;
            break;
        case VK_RETURN:
            setting.save();
            break;
        }
    }
}
void drawoptions()
{
    setbkmode(TRANSPARENT);
    settextstyle(60, 0, _T("微软雅黑"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(960 - 48, 120, _T("设置"));
    const TCHAR *labels[] = {_T("游戏类型"), _T("灵敏度")};
    TCHAR value_str[2][32];
    wsprintf(value_str[0], setting.gamekind == 0 ? _T("CS2") : _T("Valorant"));
    _stprintf(value_str[1], _T("%.2f"), setting.sensitivity);
    // wsprintf(value_str[2], _T("%d%%"), setting.volumn);

    settextstyle(40, 0, _T("微软雅黑"));

    for (int i = 0; i < 2; i++)
    {
        int y = 400 + i * 100;

        if (i == selected)
        {
            settextcolor(RGB(255, 200, 50));
            outtextxy(700, y, _T(">"));
        }
        else
        {
            settextcolor(RGB(180, 180, 180));
        }

        outtextxy(760, y, labels[i]);
        outtextxy(1120, y, value_str[i]);
    }
    settextstyle(24, 0, _T("微软雅黑"));
    settextcolor(RGB(140, 140, 140));

    outtextxy(750, 900, _T("[ESC] 回到主菜单，[↑][↓]切换选项，[←][→]改动数值"));
}