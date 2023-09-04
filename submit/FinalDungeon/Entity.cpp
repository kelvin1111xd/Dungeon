#include "Entity.h"
#include "Dungeon.h"
#include "Wizzard.h"
#include "Claw.h"

void Entity::updateFrame(int currentFrame)
{
	switch (currentStatus)
	{
	case IDLE:
		switch (currentFrame)
		{
		case 0:
			if (looking) setPixmap(texture.idle[0]);
			else setPixmap(texture.idle[0].transformed(QTransform().scale(-1, 1)));
			break;
		case 1:
			if(looking) setPixmap(texture.idle[1]);
			else setPixmap(texture.idle[1].transformed(QTransform().scale(-1, 1)));
			break;
		case 2:
			if(looking) setPixmap(texture.idle[2]);
			else setPixmap(texture.idle[2].transformed(QTransform().scale(-1, 1)));
			break;
		case 3:
			if(looking) setPixmap(texture.idle[3]);
			else setPixmap(texture.idle[3].transformed(QTransform().scale(-1, 1)));
			break;
		default:
			break;
		}
		break;
	case RUN:
		switch (currentFrame)
		{
		case 0:
			if(looking) setPixmap(texture.run[0]);
			else setPixmap(texture.run[0].transformed(QTransform().scale(-1, 1)));
			break;
		case 1:
			if(looking) setPixmap(texture.run[1]);
			else setPixmap(texture.run[1].transformed(QTransform().scale(-1, 1)));
			break;
		case 2:
			if(looking) setPixmap(texture.run[2]);
			else setPixmap(texture.run[2].transformed(QTransform().scale(-1, 1)));
			break;
		case 3:
			if(looking) setPixmap(texture.run[3]);
			else setPixmap(texture.run[3].transformed(QTransform().scale(-1, 1)));
			break;
		default:
			break;
		}
		break;
	case HIT:
		if(looking) setPixmap(texture.hit);
		else setPixmap(texture.hit.transformed(QTransform().scale(-1, 1)));
		break;
	default:
		break;
	}
}

bool Entity::tryMove(int deltaX, int deltaY)
{
	if (!(pos().x() + 32 + deltaX >= range.x + 32 && pos().x() + 32 + deltaX < range.x + range.width - 32 &&
		pos().y() + 32 - overHeight + deltaY >= range.y + 32 && pos().y() + 32 - overHeight + deltaY < range.y + range.length - 32))
		return false;

	if (dungeon->isPosValidCreature(pos().x() + deltaX, pos().y() + deltaY - overHeight))
	{
		move(pos().x() + deltaX, pos().y() + deltaY);
		return true;
	}
	else
		return false;
}

void Entity::attack()
{
	QPoint wizzardPos = dungeon->getWizzard()->pos();
	Claw* claw = new Claw();
	claw->move(wizzardPos.x(), wizzardPos.y() - dungeon->getWizzard()->getOverHeight());
	claw->setParent(dungeon);
	claw->setDungeon(dungeon);
	claw->show();

	if(dungeon->getWizzard()->getLook())
		dungeon->getWizzard()->setPixmap(dungeon->getWizzard()->getHitTexture());
	else
		dungeon->getWizzard()->setPixmap(dungeon->getWizzard()->getHitTexture().transformed(QTransform().scale(-1, 1)));
	dungeon->getWizzard()->tookDamage(damage);
}

void Entity::tookDamage(int damage)
{
	hp -= damage;
	if (hp <= 0 && id != 0)
	{
		id = -1;
	}
}