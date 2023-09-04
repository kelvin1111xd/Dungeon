#pragma once
#include <QPixmap>
#include <vector>
using namespace std;

typedef struct entityTexture
{
	vector<QPixmap> idle;
	vector<QPixmap> run;
	QPixmap hit;
} entityTexture;

enum STATUS
{
	IDLE,
	RUN,
	HIT,
};

enum LOOKLR
{
	LLEFT = false,
	LRIGHT = true,
};

enum WALLDIRECTION
{
	WU,
	WD,
	WL,
	WR,
	WCORINS_LU,
	WCORINS_RU,
	WCORINS_LD,
	WCORINS_RD,
	WCOROUTS_L,
	WCOROUTS_R,
	GROUND,
};

enum POTION
{
	BIGATK,
	SMLATK,
	BIGHEAL,
	SMLHEAL,
};

enum CHEST
{
	NORMAL,
	MIMIC,
};