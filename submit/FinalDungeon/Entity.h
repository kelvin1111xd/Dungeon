#pragma once
#include <QLabel>
#include <QTimer>
#include <QObject>
#include <conio.h>
#include <cmath>
#include "Texture.h"

typedef struct movementRange
{
	int x;
	int y;
	int width;
	int length;
} movementRange;

class Dungeon;

class Entity : public QLabel
{
protected:
	string whatIsThis = "Entity";
	Dungeon* dungeon = nullptr;
	int id = 1;
	entityTexture texture;
	int currentStatus = IDLE;
	int previousStatus = IDLE;
	bool looking = LRIGHT;
	int overHeight = 0;
	int hp = 10;
	int moveSpeed = 1;
	int damage = 1;
	int attackColdDown = 0;
	movementRange range = {0, 0, 3840, 3840};
	QTimer* actionTime = nullptr;
	int moveRandomly = 0;

public:
	Entity() {}
	~Entity()
	{
		if(actionTime != nullptr) delete(actionTime);
	}
	void setDungeon(Dungeon* _dungeon) { dungeon = _dungeon; }
	void setCurrentStatus(int status) { currentStatus = status; }
	string getWhatIsThis() { return whatIsThis; }
	void setId(int amount) { id = amount; }
	int getId() { return id; }
	void setLook(bool dir) { looking = dir; }
	bool getLook() { return looking; }
	void setHp(int amount) { hp = amount; }
	int getHp() { return hp; }
	void setDamage(int amount) { damage = amount; }
	int getDamage() { return damage; }
	movementRange& getRange() { return range; }
	QPixmap getHitTexture() { return texture.hit; }
	int& getOverHeight() { return overHeight; }
	void updateFrame(int currentFrame);

	void resetPos() { move(0, overHeight); }
	virtual bool tryMove(int deltaX, int deltaY);
	void setRange(int rx, int ry, int rw, int rl) { range = movementRange{ rx, ry, rw, rl }; }
	int getMoveSpeed() { return moveSpeed; }

	void attack();
	virtual void tookDamage(int damage);
};

