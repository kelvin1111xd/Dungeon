#include "Wizzard.h"
#include "Dungeon.h"

Wizzard::Wizzard()
{
	texture = 
	{
		{
			QPixmap(".\\Texture\\Entity\\wizzard_m_idle_anim_f0.png").scaled(64, 112, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\wizzard_m_idle_anim_f1.png").scaled(64, 112, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\wizzard_m_idle_anim_f2.png").scaled(64, 112, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\wizzard_m_idle_anim_f3.png").scaled(64, 112, Qt::KeepAspectRatio),
		},
		{
			QPixmap(".\\Texture\\Entity\\wizzard_m_run_anim_f0.png").scaled(64, 112, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\wizzard_m_run_anim_f1.png").scaled(64, 112, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\wizzard_m_run_anim_f2.png").scaled(64, 112, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\wizzard_m_run_anim_f3.png").scaled(64, 112, Qt::KeepAspectRatio),
		},
		QPixmap(".\\Texture\\Entity\\wizzard_m_hit_anim_f0.png").scaled(64, 112, Qt::KeepAspectRatio)
	};
	setPixmap(texture.idle[0]);
	resize(64, 112);
	overHeight = -48;
	resetPos();
	id = 0;
	moveSpeed = 8;
	damage = 1;
	hp = 3;
}

void Wizzard::generateHeartUI()
{
	for (int i = 0; i < hp; i++)
	{
		Heart* heart = new Heart();
		heart->setParent(dungeon);
		heart->setDungeon(dungeon);
		heart->move(16 + i * 48, 745);
		hearts.push_back(heart);
		heart->show();
	}
}

bool Wizzard::tryMove(int deltaX, int deltaY)
{
	if (dungeon->isPosValid(pos().x() + deltaX, pos().y() + deltaY - overHeight))
	{
		move(pos().x() + deltaX, pos().y() + deltaY);
		return true;
	}
	else
		return false;
}

void Wizzard::tookDamage(int damage)
{
	hp -= damage;
	if (hp <= 2 && hearts[2]->frame == 3) hearts[2]->drop();
	if (hp <= 1 && hearts[1]->frame == 3) hearts[1]->drop();
	if (hp <= 0 && hearts[0]->frame == 3) hearts[0]->drop();
}