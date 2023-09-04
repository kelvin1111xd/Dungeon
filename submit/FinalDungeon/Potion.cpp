#include "Potion.h"
#include "Dungeon.h"
#include "Wizzard.h"

Potion::Potion(int type)
{
	whatIsThis = "Potion";
	this->type = type;
	switch (type)
	{
	case BIGATK:
		setPixmap(QPixmap("./Texture/Object/big_blue_potion.png").scaled(64, 64, Qt::KeepAspectRatio));
		break;
	case SMLATK:
		setPixmap(QPixmap("./Texture/Object/blue_potion.png").scaled(64, 64, Qt::KeepAspectRatio));
		break;
	case BIGHEAL:
		setPixmap(QPixmap("./Texture/Object/big_red_potion.png").scaled(64, 64, Qt::KeepAspectRatio));
		break;
	case SMLHEAL:
		setPixmap(QPixmap("./Texture/Object/red_potion.png").scaled(64, 64, Qt::KeepAspectRatio));
		break;
	default:
		break;
	}
	resize(64, 64);
	move(0, 0);
	id = rand() % 100000000;
	eventTimer = new QTimer(this);
	eventTimer->setInterval(50);
	QObject::connect(eventTimer, &QTimer::timeout, [&]() 
		{
			QPoint wizzardPos = dungeon->getWizzard()->pos();
			if (wizzardPos.x() + 32 >= pos().x() - 64 && wizzardPos.x() + 32 <= pos().x() + 128 &&
				wizzardPos.y() - dungeon->getWizzard()->getOverHeight() + 32 >= pos().y() - 64 &&
				wizzardPos.y() - dungeon->getWizzard()->getOverHeight() + 32 <= pos().y() + 128)
			{
				if (eHint == nullptr)
				{
					generatePressHint();
					trigger = true;
				}
			}
			else if (eHint != nullptr)
			{
				delete(eHint);
				eHint = nullptr;
				trigger = false;
			}
		});
	eventTimer->start();
}

void Potion::triggerEvent()
{
	Wizzard* wizzard = dungeon->getWizzard();
	switch (type)
	{
	case BIGATK:
		wizzard->setDamage(wizzard->getDamage() + 2);
		break;
	case SMLATK:
		wizzard->setDamage(wizzard->getDamage() + 1);
		break;
	case BIGHEAL:
		wizzard->setHp(wizzard->getHp() + 2);
		if (wizzard->getHp() > 3) wizzard->setHp(3);
		for (int i = 0; i < wizzard->getHeart().size(); i++)
		{
			if(wizzard->getHp() >= i + 1) wizzard->getHeart()[i]->fill();
		}
		break;
	case SMLHEAL:
		wizzard->setHp(wizzard->getHp() + 1);
		if (wizzard->getHp() > 3) wizzard->setHp(3);
		for (int i = 0; i < wizzard->getHeart().size(); i++)
		{
			if (wizzard->getHp() >= i + 1) wizzard->getHeart()[i]->fill();
		}
		break;
	default:
		break;
	}
	id = -1;
	trigger = false;
}