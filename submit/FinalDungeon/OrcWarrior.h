#pragma once
#include "Entity.h"
class OrcWarrior :
    public Entity
{
public:
    OrcWarrior();

    void action();
    bool seePlayer();
};

