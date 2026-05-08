#pragma once
#include "common.h"
#include "camera.h"
#include "setting.h"

struct Ball_follow
{
    float world_x = 0;
    float world_y = 0;
    float radius = 15;
};

extern Ball_follow ball_follow;