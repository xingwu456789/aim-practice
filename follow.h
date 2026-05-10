#pragma once
#include "common.h"
#include "camera.h"
#include "setting.h"

struct Ball_follow
{
    float world_x = 0;
    float world_y = 0;
    float radius = 20;
    float target_x = 0;
    float target_y = 0;
    float speed = 200.0f;
    float wait_timer = 0;
};

extern Ball_follow ball_follow;

void initfollow();
void updatefollow(float dt);
void followprocess(const ExMessage &msg);
void drawfollow();
