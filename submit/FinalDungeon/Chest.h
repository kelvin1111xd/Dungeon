#pragma once
#include "Object.h"
class Chest :
    public Object
{
public:
    Chest() {}
    Chest(int type);

    void triggerEvent() override;
};

