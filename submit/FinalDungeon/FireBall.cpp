#include "FireBall.h"
#include "Dungeon.h"
#include "Wizzard.h"
#include <QPainter>

FireBall::FireBall(int ax, int ay, int direction)
{
	texture =
	{
		{
			QPixmap(".\\Texture\\Entity\\fireball_f0.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
			QPixmap(".\\Texture\\Entity\\fireball_f1.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
			QPixmap(".\\Texture\\Entity\\fireball_f2.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
			QPixmap(".\\Texture\\Entity\\fireball_f3.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
		},
		{
			QPixmap(".\\Texture\\Entity\\fireball_hit_f0.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
			QPixmap(".\\Texture\\Entity\\fireball_hit_f1.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
			QPixmap(".\\Texture\\Entity\\fireball_hit_f2.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
			QPixmap(".\\Texture\\Entity\\fireball_hit_f3.png").scaled(57, 57, Qt::KeepAspectRatio).transformed(QTransform().rotate(-45 * direction)),
		}
	};
	setPixmap(texture.idle[0]);
	resize(57, 57);
	resetPos();
	moveSpeed = 20;
	int temp = round((double)moveSpeed / 1.414);
	switch (direction)
	{
	case DR:
		dx = moveSpeed;
		move(ax + 64, ay);
		break;
	case DRU:
		dx = temp;
		dy = -temp;
		move(ax + 64, ay - 64);
		break;
	case DU:
		dy = -moveSpeed;
		move(ax, ay - 64);
		break;
	case DLU:
		dx = -temp;
		dy = -temp;
		move(ax - 64, ay - 64);
		break;
	case DL:
		dx = -moveSpeed;
		move(ax - 64, ay);
		break;
	case DLD:
		dx = -temp;
		dy = temp;
		move(ax - 64, ay + 64);
		break;
	case DD:
		dy = moveSpeed;
		move(ax, ay + 64);
		break;
	case DRD:
		dx = temp;
		dy = temp;
		move(ax + 64, ay + 64);
		break;
	}

	actionTime = new QTimer(this);
	actionTime->setInterval(50);
	QObject::connect(actionTime, &QTimer::timeout, [&]()
		{
			if (!hit) moving();
			else hitProgress();
		});
	actionTime->start();
}

bool FireBall::tryMove(int deltaX, int deltaY)
{
	if (dungeon->isPosValid(pos().x() + deltaX, pos().y() + deltaY - overHeight))
	{
		move(pos().x() + deltaX, pos().y() + deltaY);
		return true;
	}
	else
		return false;
}

void FireBall::moving()
{
	if (tryMove(dx, dy))
	{
		updateFrame(frame);
		frame++;
		if (frame == 4) frame = 0;
	}
	else
	{
		hit = true;
		move(pos().x() + dx, pos().y() + dy);
		currentStatus = RUN;
		frame = 0;

		for (auto& creature : dungeon->getCreatures())
			if ((pos().x() + 64 > creature->pos().x() && pos().x() < creature->pos().x() + 64) &&
				(pos().y() + 64 > creature->pos().y() - creature->getOverHeight() && pos().y() < creature->pos().y() + 64 - creature->getOverHeight()))
			{
				if(creature->getLook())
					creature->setPixmap(creature->getHitTexture());
				else
					creature->setPixmap(creature->getHitTexture().transformed(QTransform().scale(-1, 1)));
				creature->tookDamage(damage);
			}
	}
}

void FireBall::hitProgress()
{
	updateFrame(frame);
	frame++;
	if (frame == 4) delete(this);
}