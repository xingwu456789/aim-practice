#include "setting.h"
#include <cstdio>
void Setting::load()
{
    FILE *f = fopen("settings.cfg", "r");
    if (!f)
        return;
    fscanf(f, "sensitivity=%f\n", &sensitivity);
    fscanf(f, "gamekind=%d\n", &gamekind);
    fscanf(f, "volumn=%d\n", &volumn);
    fclose(f);
}
void Setting::save()
{
    FILE *f = fopen("settings.cfg", "w");
    if (!f)
    {
        return;
    }
    fprintf(f, "sensitivity=%f\n", sensitivity);
    fprintf(f, "gamekind=%d\n", gamekind);
    fprintf(f, "volumn=%d\n", volumn);
    fclose(f);
}
float Setting::getsen()
{
    float yaw = (gamekind == 0) ? 0.022f : 0.069f;
    float fov = (gamekind == 0) ? 90.0f : 103.0f;
    return yaw * sensitivity * (1920.0f / fov);
}