#pragma once
#include "Object.h"
class DungeonGround :
    public Object
{
private:
    vector<QPixmap> texture;

public:
    DungeonGround();
};

