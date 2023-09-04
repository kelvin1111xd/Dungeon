#include "DungeonWall.h"
#include <conio.h>
#include <ctime>

DungeonWall::DungeonWall(int type)
{
	this->type = type;
	vector<QPixmap> texture;

	switch (type)
	{
	case WU:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(16, 0, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(32, 0, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(48, 0, 16, 16));
		break;
	case WD:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(16, 64, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(16, 80, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(32, 64, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(32, 80, 16, 16));
		break;
	case WL:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(0, 16, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(0, 32, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(0, 48, 16, 16));
		break;
	case WR:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(80, 16, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(80, 32, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(80, 48, 16, 16));
		break;
	case WCORINS_LU:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(0, 0, 16, 16));
		break;
	case WCORINS_RU:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(80, 0, 16, 16));
		break;
	case WCORINS_LD:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(0, 64, 16, 16));
		break;
	case WCORINS_RD:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(80, 64, 16, 16));
		break;
	case WCOROUTS_L:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(0, 80, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(64, 80, 16, 16));
		break;
	case WCOROUTS_R:
		texture.push_back(QPixmap("./Texture/tileset.png").copy(48, 80, 16, 16));
		texture.push_back(QPixmap("./Texture/tileset.png").copy(80, 80, 16, 16));
		break;
	default:
		break;
	}

	resize(64, 64);
	setPixmap(texture[rand() % texture.size()].scaled(64, 64, Qt::KeepAspectRatio));
}