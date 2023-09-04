#pragma once
#include <QLabel>
#include <QTimer>
#include <cmath>
#include <conio.h>
#include "Texture.h"

class Dungeon;

class Object : public QLabel
{
protected:
	string whatIsThis = "Object";
	int id = 1;
	Dungeon* dungeon = nullptr;
	QLabel* eHint = nullptr;
	QTimer* eventTimer = nullptr;

public:
	int type = -1;
	bool trigger = false;

	Object() {}
	~Object()
	{
		if(eHint != nullptr) delete(eHint);
		if(eventTimer != nullptr) delete(eventTimer);
	}

	void setDungeon(Dungeon* _dungeon) { dungeon = _dungeon; }
	string getWhatIsThis() { return whatIsThis; }
	void setId(int amount) { id = amount; }
	int getId() { return id; }

	void generatePressHint();
	virtual void triggerEvent() {}
};