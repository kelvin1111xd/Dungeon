#pragma once
#include "Entity.h"
class Skull :
    public Entity
{
private:
    bool seeFlag = false;

public:
    Skull();

    void action();
    bool seePlayer();
};

