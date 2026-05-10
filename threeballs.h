#pragma once
#include "common.h"
#include "camera.h"
#include "setting.h"

void spawnball(int index);
void initthreeballs();
void processthreeballs(const ExMessage &msg);
void drawthreeballs();

struct Ball
{
    float world_x = 0;
    float world_y = 0;
    float radius = 18;
};

extern Ball balls[3];