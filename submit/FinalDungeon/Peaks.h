#pragma once
#include "Entity.h"
class Peaks :
    public Entity
{
public:
    bool outside = true;
    int comeOut = 0;
    Peaks();

    void action();
    bool seePlayer();
};

