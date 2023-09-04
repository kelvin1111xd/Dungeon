#pragma once
#include "Object.h"
class Potion :
    public Object
{
public:
    Potion() {}
    Potion(int type);

    void triggerEvent() override;
};

