#pragma once
#include "Entity.h"
#include "Heart.h"

class Wizzard : public Entity
{
private:
	vector<Heart*> hearts;

public:
	Wizzard();
	~Wizzard()
	{
		for (int i = hearts.size() - 1; i >= 0; i--)
			delete(hearts[i]);
	}
	void generateHeartUI();
	vector<Heart*>& getHeart() { return hearts; }

	bool tryMove(int deltaX, int deltaY) override;
	void tookDamage(int damage) override;
};