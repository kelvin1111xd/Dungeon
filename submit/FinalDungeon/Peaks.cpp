#include "Peaks.h"
#include "Dungeon.h"
#include "Wizzard.h"

Peaks::Peaks()
{
	whatIsThis = "OrcWarrior";
	texture =
	{
		{
			QPixmap(".\\Texture\\Entity\\peaks_f0.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\peaks_f1.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\peaks_f2.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\peaks_f3.png").scaled(64, 64, Qt::KeepAspectRatio),
		},
		{
			QPixmap(".\\Texture\\Entity\\peaks_f0.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\peaks_f1.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\peaks_f2.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\peaks_f3.png").scaled(64, 64, Qt::KeepAspectRatio),
		},
		QPixmap(".\\Texture\\Entity\\orc_warrior_hit.png").scaled(64, 64, Qt::KeepAspectRatio)
	};
	setPixmap(texture.idle[0]);
	resize(64, 64);
	resetPos();
	id = rand() % 100000000;
	hp = 999999;

	actionTime = new QTimer(this);
	actionTime->setInterval(50);
	QObject::connect(actionTime, &QTimer::timeout, [&]()
		{
			action();
		});
	actionTime->start();
}

void Peaks::action()
{
	if (attackColdDown > 0) attackColdDown--;
	if (comeOut > 0) comeOut--;
	if (seePlayer())
	{
		if (abs((dungeon->getWizzard()->pos().y() - dungeon->getWizzard()->getOverHeight() + 32) - (pos().y() - overHeight + 32)) <= 48
			&& abs(dungeon->getWizzard()->pos().x() + 32 - (pos().x() + 32)) <= 48 && outside)
		{
			currentStatus = IDLE;
			if (attackColdDown == 0)
			{
				attack();
				attackColdDown = 20;
			}
			return;
		}
	}

	if (comeOut == 0 && outside)
	{
		outside = false;
		updateFrame(3);
		comeOut = 60;
	}
	else if (comeOut == 0 && !outside)
	{
		outside = true;
		updateFrame(0);
		comeOut = 60;
	}
}

bool Peaks::seePlayer()
{
	Wizzard* wizzard = dungeon->getWizzard();
	if (wizzard->pos().x() + 32 >= range.x && wizzard->pos().x() + 32 < range.x + range.width &&
		wizzard->pos().y() + 32 - wizzard->getOverHeight() >= range.y &&
		wizzard->pos().y() + 32 - wizzard->getOverHeight() < range.y + range.length)
		return true;
	else
		return false;
}