#include "Skull.h"
#include "Dungeon.h"
#include "Wizzard.h"

Skull::Skull()
{
	whatIsThis = "Skull";
	texture =
	{
		{
			QPixmap(".\\Texture\\Entity\\skull_f0.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\skull_f1.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\skull_f2.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\skull_f3.png").scaled(64, 64, Qt::KeepAspectRatio),
		},
		{
			QPixmap(".\\Texture\\Entity\\skull_f0.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\skull_f1.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\skull_f2.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\skull_f3.png").scaled(64, 64, Qt::KeepAspectRatio),
		},
		QPixmap(".\\Texture\\Entity\\skull_hit.png").scaled(64, 64, Qt::KeepAspectRatio)
	};
	setPixmap(texture.idle[0]);
	resize(64, 64);
	resetPos();
	id = rand() % 100000000;
	moveSpeed = 4;
	hp = 4;

	actionTime = new QTimer(this);
	actionTime->setInterval(50);
	QObject::connect(actionTime, &QTimer::timeout, [&]()
		{
			action();
		});
	actionTime->start();
}

void Skull::action()
{
	if (seePlayer())
	{
		if (abs((dungeon->getWizzard()->pos().y() - dungeon->getWizzard()->getOverHeight() + 32) - (pos().y() - overHeight + 32)) <= 80
			&& abs(dungeon->getWizzard()->pos().x() + 32 - (pos().x() + 32)) <= 80)
		{
			currentStatus = IDLE;
			if (attackColdDown == 0)
			{
				attack();
				attackColdDown = 20;
			}
			else
			{
				attackColdDown--;
			}
			return;
		}

		currentStatus = RUN;
		moveRandomly = 0;

		double deltaX, deltaY;
		deltaX = abs(moveSpeed * cos(atan((double)(dungeon->getWizzard()->pos().y() + dungeon->getWizzard()->getOverHeight()
			- pos().y() - overHeight) / (double)(dungeon->getWizzard()->pos().x() - pos().x()))));
		deltaY = abs(moveSpeed * sin(atan((double)(dungeon->getWizzard()->pos().y() + dungeon->getWizzard()->getOverHeight()
			- pos().y() - overHeight) / (double)(dungeon->getWizzard()->pos().x() - pos().x()))));

		looking = LRIGHT;
		if (dungeon->getWizzard()->pos().x() < pos().x())
		{
			looking = LLEFT;
			deltaX = -deltaX;
		}
		if (dungeon->getWizzard()->pos().y() < pos().y()) deltaY = -deltaY;
		move(pos().x() + deltaX, pos().y() + deltaY);
	}
}

bool Skull::seePlayer()
{
	Wizzard* wizzard = dungeon->getWizzard();
	if (seeFlag)
		return true;
	else if (wizzard->pos().x() + 32 >= range.x && wizzard->pos().x() + 32 < range.x + range.width &&
		wizzard->pos().y() + 32 - wizzard->getOverHeight() >= range.y &&
		wizzard->pos().y() + 32 - wizzard->getOverHeight() < range.y + range.length)
	{
		seeFlag = true;
		return true;
	}
	else
		return false;
}