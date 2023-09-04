#include "Chest.h"
#include "Dungeon.h"
#include "Wizzard.h"
#include "Potion.h"
#include "Mimic.h"

Chest::Chest(int type)
{
	whatIsThis = "Chest";
	this->type = type;
	setPixmap(QPixmap("./Texture/Object/chest.png").scaled(64, 64, Qt::KeepAspectRatio));
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

void Chest::triggerEvent()
{
	Wizzard* wizzard = dungeon->getWizzard();
	Potion* randPotion;
	Mimic* mimic;
	switch (type)
	{
	case NORMAL:
		randPotion = new Potion(rand() % 4);
		randPotion->setParent(dungeon);
		randPotion->setDungeon(dungeon);
		randPotion->move(pos());
		randPotion->show();
		dungeon->getObjects().push_back(randPotion);
		break;
	case MIMIC:
		mimic = new Mimic();
		mimic->setParent(dungeon);
		mimic->setDungeon(dungeon);
		mimic->move(pos());
		mimic->show();
		dungeon->getCreatures().push_back(mimic);
		mimic->attack();
		break;
	default:
		break;
	}
	id = -1;
	setVisible(false);
	trigger = false;
}