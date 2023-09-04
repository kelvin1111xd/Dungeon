#include "OrcWarrior.h"
#include "Dungeon.h"
#include "Wizzard.h"

OrcWarrior::OrcWarrior()
{
	whatIsThis = "OrcWarrior";
	texture =
	{
		{
			QPixmap(".\\Texture\\Entity\\orc_warrior_idle_anim_f0.png").scaled(64, 92, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\orc_warrior_idle_anim_f1.png").scaled(64, 92, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\orc_warrior_idle_anim_f2.png").scaled(64, 92, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\orc_warrior_idle_anim_f3.png").scaled(64, 92, Qt::KeepAspectRatio),
		},
		{
			QPixmap(".\\Texture\\Entity\\orc_warrior_run_anim_f0.png").scaled(64, 92, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\orc_warrior_run_anim_f1.png").scaled(64, 92, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\orc_warrior_run_anim_f2.png").scaled(64, 92, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\orc_warrior_run_anim_f3.png").scaled(64, 92, Qt::KeepAspectRatio),
		},
		QPixmap(".\\Texture\\Entity\\orc_warrior_hit.png").scaled(64, 92, Qt::KeepAspectRatio)
	};
	setPixmap(texture.idle[0]);
	resize(64, 92);
	overHeight = -28;
	resetPos();
	id = rand() % 100000000;
	moveSpeed = 4;
	hp = 8;

	actionTime = new QTimer(this);
	actionTime->setInterval(50);
	QObject::connect(actionTime, &QTimer::timeout, [&]() 
		{
			action();
		});
	actionTime->start();
}

void OrcWarrior::action()
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
		if (tryMove(round(deltaX), round(deltaY)));
		else
		{
			if (deltaX > 0) deltaX = moveSpeed;
			else deltaX = -moveSpeed;
			if (deltaY > 0) deltaY = moveSpeed;
			else deltaY = -moveSpeed;

			if (tryMove(deltaX, 0));
			else tryMove(0, deltaY);
		}
	}
	else if (currentStatus == RUN && moveRandomly % 100 == 0)
	{
		moveRandomly = 0;
		currentStatus = IDLE;
	}
	else if (currentStatus == RUN && moveRandomly % 100 != 0)
	{
		moveRandomly--;
		if (moveRandomly / 100 == 0)
		{
			tryMove(moveSpeed, 0);
			looking = LRIGHT;
		}
		else if (moveRandomly / 100 == 1)
		{
			tryMove(-moveSpeed, 0);
			looking = LLEFT;
		}
		else if (moveRandomly / 100 == 2)
		{
			tryMove(0, moveSpeed);
		}
		else if (moveRandomly / 100 == 3)
		{
			tryMove(0, -moveSpeed);
		}
		else if (moveRandomly / 100 == 4)
		{
			tryMove(round((double)moveSpeed / 1.414), round((double)moveSpeed / 1.414));
			looking = LRIGHT;
		}
		else if (moveRandomly / 100 == 5)
		{
			tryMove(-round((double)moveSpeed / 1.414), round((double)moveSpeed / 1.414));
			looking = LLEFT;
		}
		else if (moveRandomly / 100 == 6)
		{
			tryMove(round((double)moveSpeed / 1.414), -round((double)moveSpeed / 1.414));
			looking = LRIGHT;
		}
		else if (moveRandomly / 100 == 7)
		{
			tryMove(-round((double)moveSpeed / 1.414), -round((double)moveSpeed / 1.414));
			looking = LLEFT;
		}

	}
	else if (currentStatus == IDLE && rand() % 100 == 0)
	{
		currentStatus = RUN;
		moveRandomly += rand() % 80 + 20;
		moveRandomly += rand() % 8 * 100;
	}
}

bool OrcWarrior::seePlayer()
{
	Wizzard* wizzard = dungeon->getWizzard();
	if (wizzard->pos().x() + 32 >= range.x && wizzard->pos().x() + 32 < range.x + range.width &&
		wizzard->pos().y() + 32 - wizzard->getOverHeight() >= range.y &&
		wizzard->pos().y() + 32 - wizzard->getOverHeight() < range.y + range.length)
		return true;
	else
		return false;
}