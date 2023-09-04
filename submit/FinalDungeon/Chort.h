#pragma once
#include "Entity.h"
class Chort :
    public Entity
{
public:
    Chort();

    void action();
    bool seePlayer();
};

