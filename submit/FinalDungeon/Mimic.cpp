#include "Mimic.h"
#include "Dungeon.h"
#include "Wizzard.h"

Mimic::Mimic()
{
	whatIsThis = "Mimic";
	texture =
	{
		{
			QPixmap(".\\Texture\\Entity\\chest_mimic_open_anim_f0.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\chest_mimic_open_anim_f1.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\chest_mimic_open_anim_f2.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\chest_mimic_open_anim_f3.png").scaled(64, 64, Qt::KeepAspectRatio),
		}
	};
	setPixmap(texture.idle[0]);
	resize(64, 64);
	resetPos();
	id = rand() % 100000000;
	moveSpeed = 1;
	hp = 1;
}