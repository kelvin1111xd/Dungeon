#include "DungeonGround.h"
#include <conio.h>

DungeonGround::DungeonGround()
{
	type = GROUND;

	texture =
	{
		QPixmap("./Texture/tileset.png").copy(96, 0, 16, 16),
		QPixmap("./Texture/tileset.png").copy(112, 0, 16, 16),
		QPixmap("./Texture/tileset.png").copy(128, 0, 16, 16),
		QPixmap("./Texture/tileset.png").copy(144, 0, 16, 16),
		QPixmap("./Texture/tileset.png").copy(96, 16, 16, 16),
		QPixmap("./Texture/tileset.png").copy(112, 16, 16, 16),
		QPixmap("./Texture/tileset.png").copy(128, 16, 16, 16),
		QPixmap("./Texture/tileset.png").copy(144, 16, 16, 16),
		QPixmap("./Texture/tileset.png").copy(96, 32, 16, 16),
		QPixmap("./Texture/tileset.png").copy(112, 32, 16, 16),
		QPixmap("./Texture/tileset.png").copy(128, 32, 16, 16),
		QPixmap("./Texture/tileset.png").copy(144, 32, 16, 16),
	};

	resize(64, 64);
	setPixmap(texture[rand() % texture.size()].scaled(64, 64, Qt::KeepAspectRatio));
}