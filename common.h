#pragma once
#include <graphics.h>

enum class GameMode
{
    reaction,
    threeballs,
    follow
};

const int window_width = 1920;
const int window_height = 1080;
const int button_width = 200;
const int button_height = 91;

extern bool running;
extern bool is_game_started;
extern bool is_options_open;
extern bool is_reaction;
extern GameMode game_mode;


