#pragma once
#include "Entity.h"
#include "Direction.h"

class FireBall :
    public Entity
{
private:
    int dx = 0;
    int dy = 0;
    int frame = 0;
    bool hit = false;

public:
    FireBall() {}
    FireBall(int ax, int ay, int direction);

    bool tryMove(int deltaX, int deltaY) override;
    void moving();
    void hitProgress();
};

