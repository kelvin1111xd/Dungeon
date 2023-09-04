#pragma once
#include "Entity.h"
class Heart :
    public Entity
{
public:
    int frame = 3;

    Heart();

    void fill();
    void drop();
};

