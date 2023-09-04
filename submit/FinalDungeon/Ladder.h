#pragma once
#include "Object.h"
class Ladder :
    public Object
{
public:
    Ladder();

    void triggerEvent() override;
};

