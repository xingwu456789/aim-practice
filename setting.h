#pragma once
struct Setting
{
    float sensitivity = 1.0;
    int gamekind = 1;
    // int volumn = 100;
    void load();
    void save();
    float getsen();
};
extern Setting setting;