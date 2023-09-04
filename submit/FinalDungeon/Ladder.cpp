#include "Ladder.h"
#include "Dungeon.h"
#include "Wizzard.h"

Ladder::Ladder()
{
	whatIsThis = "Ladder";
	setPixmap(QPixmap("./Texture/tileset.png").copy(144, 48, 16, 16).scaled(64, 64, Qt::KeepAspectRatio));
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

void Ladder::triggerEvent()
{
	id = -1;
	setVisible(false);
	trigger = false;
	dungeon->nextLevel();
}