#pragma once
#include "Entity.h"
class PumpkinDude :
    public Entity
{
public:
    PumpkinDude();

    void action();
    bool seePlayer();
};

