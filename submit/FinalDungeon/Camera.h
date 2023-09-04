#pragma once
#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QKeyEvent>
#include <QTimer>
#include <fstream>

enum KEYPRESSING
{
    EW,
    EA,
    ES,
    ED,
    EE,
    EUP,
    ELEFT,
    EDOWN,
    ERIGHT,
    ESPACE,
    ESAVE,
    ELOAD,
    EINVALID,
};

class Dungeon;

class Camera :
    public QScrollArea
{
private:
    Dungeon* dungeon = nullptr;
    QTimer* frame = nullptr;
    int coldDown = 0;
    int eColdDown = 0;

public:
    Camera() {}
    Camera(Dungeon* dungeon) : QScrollArea() 
    {
        intialize(dungeon);
    }

    bool keyState[EINVALID]{ false };

    void intialize(Dungeon* dungeon);
    bool isScrollValid(int direction);
    void resetScroll();
    void keyUpdate();
    void save();
    void load();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

