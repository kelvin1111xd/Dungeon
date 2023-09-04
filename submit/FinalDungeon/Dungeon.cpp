#include "Dungeon.h"
#include "Entity.h"
#include "Wizzard.h"
#include "OrcWarrior.h"
#include "Chort.h"
#include "DungeonWall.h"
#include "DungeonGround.h"
#include "Direction.h"
#include "FireBall.h"
#include "Smoke.h"
#include "Potion.h"
#include "Chest.h"
#include "Mimic.h"
#include "Ladder.h"
#include "Skull.h"
#include "PumpkinDude.h"
#include "Peaks.h"
#include <QPushButton>
#include <QMessageBox>

Dungeon::Dungeon()
{
	srand(time(NULL));
	resize(3840, 3840);

	camera = new Camera(this);

	camera->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	camera->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	camera->setFixedSize(800, 800);
	camera->move(0, 0);

	bgm = new QMediaPlayer(this);
	bgm->setMedia(QUrl("qrc:/Dungeon/Texture/bgm.mp3"));
	bgm->play();


	backGround = new QLabel(this);
	backGround->move(0, 0);
	backGround->resize(3840, 3840);
	backGround->setPixmap(QPixmap("./Texture/background.png"));

	wizzard = new Wizzard();
	wizzard->setParent(this);
	wizzard->setDungeon(this);

	wizzard->generateHeartUI();

	firstFloor();

	camera->setWidget(this);
	camera->show();

	animation = new QTimer();
	animation->setParent(this);
	animation->setInterval(200);
	QObject::connect(animation, &QTimer::timeout, [&]()
		{
			for (int i = creatures.size() - 1; i >= 0; i--)
				if (creatures[i]->getId() == -1)
				{
					Smoke* smoke = new Smoke();
					smoke->move(creatures[i]->pos().x(), creatures[i]->pos().y() - creatures[i]->getOverHeight());
					smoke->setParent(this);
					smoke->setDungeon(this);
					smoke->show();

					delete(creatures[i]);
					creatures.erase(creatures.begin() + i);
				}

			for (int i = objects.size() - 1; i >= 0; i--)
				if (objects[i]->getId() == -1)
				{
					delete(objects[i]);
					objects.erase(objects.begin() + i);
				}

			for (int i = peaks.size() - 1; i >= 0; i--)
				if (peaks[i]->getId() == -1)
				{
					delete(peaks[i]);
					peaks.erase(peaks.begin() + i);
				}

			if (creatures.size() == 0 && level == 3)
			{
				QWidget gameover;
				QPushButton button("Quit", &gameover);
				QMessageBox::information(&gameover, "Game Over!", "You Win!");
				app->quit();
				gameover.show();
			}

			worldFrame++;
			if (worldFrame == 4) worldFrame = 0;

			if (resetColdDown != 0)
			{
				resetColdDown--;
				if (resetColdDown == 0)
				{
					wizzard->setHp(3);
					for (int i = 0; i < wizzard->getHeart().size(); i++)
					{
						wizzard->getHeart()[i]->fill();
						wizzard->getHeart()[i]->move(16 + i * 48, 745);
					}
					wizzard->resetPos();
					wizzard->move(wizzard->pos().x() + 320, wizzard->pos().y() + 320);
					camera->verticalScrollBar()->setValue(0);
					camera->horizontalScrollBar()->setValue(0);
				}
			}

			if(wizzard->getHp() > 0) wizzard->updateFrame(worldFrame);
			else if(wizzard->getHp() <= 0 && resetColdDown == 0)
			{
				Smoke* smoke = new Smoke();
				smoke->move(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight());
				smoke->setParent(this);
				smoke->setDungeon(this);
				smoke->show();

				wizzard->setPixmap(QPixmap());

				resetColdDown = 6;
			}

			for (auto& creature : creatures) creature->updateFrame(worldFrame);
		});

	animation->start();
}

void Dungeon::firstFloor()
{
	board.clear();

	board =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	wizzard->resetPos();
	wizzard->move(wizzard->pos().x() + 320, wizzard->pos().y() + 320);

	generateMap();

	OrcWarrior* orc1 = new OrcWarrior();
	orc1->setParent(this);
	orc1->setDungeon(this);
	orc1->move(orc1->pos().x() + 1088, orc1->pos().y() + 320);
	orc1->setRange(896, 128, 7 * 64, 17 * 64);
	creatures.push_back(orc1);
	orc1->show();

	for (int i = 0; i < 3; i++)
	{
		Peaks* peak = new Peaks();
		peak->setParent(this);
		peak->setDungeon(this);
		peak->move((16 + i) * 64, 21 * 64);
		peaks.push_back(peak);
		peak->show();
	}

	OrcWarrior* orc2 = new OrcWarrior();
	orc2->setParent(this);
	orc2->setDungeon(this);
	orc2->move(orc2->pos().x() + 320, orc2->pos().y() + 1728);
	orc2->setRange(128, 1536, 29 * 64, 7 * 64);
	creatures.push_back(orc2);
	orc2->show();

	OrcWarrior* orc3 = new OrcWarrior();
	orc3->setParent(this);
	orc3->setDungeon(this);
	orc3->move(orc3->pos().x() + 1088, orc3->pos().y() + 1728);
	orc3->setRange(128, 1536, 29 * 64, 7 * 64);
	creatures.push_back(orc3);
	orc3->show();

	Chort* chort1 = new Chort();
	chort1->setParent(this);
	chort1->setDungeon(this);
	chort1->move(chort1->pos().x() + 1792, chort1->pos().y() + 1728);
	chort1->setRange(128, 1536, 29 * 64, 7 * 64);
	creatures.push_back(chort1);
	chort1->show();

	Potion* healer1 = new Potion(SMLHEAL);
	healer1->setParent(this);
	healer1->setDungeon(this);
	healer1->move(5 * 64, 15 * 64);
	objects.push_back(healer1);
	healer1->show();

	Chest* chest1 = new Chest(NORMAL);
	chest1->setParent(this);
	chest1->setDungeon(this);
	chest1->move(27 * 64, 6 * 64);
	objects.push_back(chest1);
	chest1->show();

	Ladder* ladder = new Ladder();
	ladder->setParent(this);
	ladder->setDungeon(this);
	ladder->move(27 * 64, 3 * 64);
	objects.push_back(ladder);
	ladder->show();

	wizzard->raise();
	for (int i = 0; i < wizzard->getHeart().size(); i++)
	{
		wizzard->getHeart()[i]->raise();
		wizzard->getHeart()[i]->move(16 + i * 48, 745);
	}
}

void Dungeon::secondFloor()
{
	board.clear();

	board =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0},
		{0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	wizzard->resetPos();
	wizzard->move(wizzard->pos().x() + 320, wizzard->pos().y() + 320);

	generateMap();

	for (int i = 0; i < 3; i++)
	{
		OrcWarrior* orc = new OrcWarrior();
		orc->setParent(this);
		orc->setDungeon(this);
		orc->setRange(14 * 64, 2 * 64, 17 * 64, 17 * 64);
		orc->move(14 * 64 + ((rand() % 15 + 1) * 64), 2 * 64 + ((rand() % 15 + 1) * 64));
		creatures.push_back(orc);
		orc->show();
	}

	for (int i = 0; i < 2; i++)
	{
		Chort* chort = new Chort();
		chort->setParent(this);
		chort->setDungeon(this);
		chort->setRange(14 * 64, 2 * 64, 17 * 64, 17 * 64);
		chort->move(14 * 64 + ((rand() % 15 + 1) * 64), 2 * 64 + ((rand() % 15 + 1) * 64));
		creatures.push_back(chort);
		chort->show();
	}

	Skull* skull1 = new Skull();
	skull1->setParent(this);
	skull1->setDungeon(this);
	skull1->move(16 * 64, 27 * 64);
	skull1->setRange(14 * 64, 24 * 64, 5 * 64, 7 * 64);
	creatures.push_back(skull1);
	skull1->show();

	for (int i = 0; i < 3; i++)
	{
		Peaks* peak = new Peaks();
		peak->setParent(this);
		peak->setDungeon(this);
		peak->move(11 * 64, (26 + i) * 64);
		peaks.push_back(peak);
		peak->show();
	}

	for (int i = 0; i < 3; i++)
	{
		Peaks* peak = new Peaks();
		peak->setParent(this);
		peak->setDungeon(this);
		peak->move((26 + i) * 64, 21 * 64);
		peaks.push_back(peak);
		peak->show();
	}

	Potion* potion = new Potion(BIGHEAL);
	potion->setParent(this);
	potion->setDungeon(this);
	potion->move(27 * 64, 27 * 64);
	objects.push_back(potion);
	potion->show();

	Chest* chest = new Chest(MIMIC);
	chest->setParent(this);
	chest->setDungeon(this);
	chest->move(5 * 64, 27 * 64);
	objects.push_back(chest);
	chest->show();

	Ladder* ladder = new Ladder();
	ladder->setParent(this);
	ladder->setDungeon(this);
	ladder->move(5 * 64, 14 * 64);
	objects.push_back(ladder);
	ladder->show();

	wizzard->raise();
	for (int i = 0; i < wizzard->getHeart().size(); i++)
	{
		wizzard->getHeart()[i]->raise();
		wizzard->getHeart()[i]->move(16 + i * 48, 745);
	}
}

void Dungeon::thirdFloor()
{
	board.clear();

	board =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
		{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0},
		{0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	wizzard->resetPos();
	wizzard->move(wizzard->pos().x() + 320, wizzard->pos().y() + 320);

	generateMap();

	//object
	for(int i = 0; i < 5; i++)
		for (int j = 0; j < 7; j++)
		{
			Peaks* peak = new Peaks();
			peak->setParent(this);
			peak->setDungeon(this);
			peak->move((18 + (i * 3)) * 64, (2 + j) * 64);
			peaks.push_back(peak);
			peak->show();
		}

	Chest* chest1 = new Chest(MIMIC);
	chest1->setParent(this);
	chest1->setDungeon(this);
	chest1->move(37 * 64, 5 * 64);
	objects.push_back(chest1);
	chest1->show();

	Chest* chest2 = new Chest(NORMAL);
	chest2->setParent(this);
	chest2->setDungeon(this);
	chest2->move(37 * 64, 17 * 64);
	objects.push_back(chest2);
	chest2->show();

	PumpkinDude* pumpkin = new PumpkinDude();
	pumpkin->setParent(this);
	pumpkin->setDungeon(this);
	pumpkin->move(5 * 64, 17 * 64);
	pumpkin->setRange(2 * 64, 14 * 64, 27 * 64, 7 * 64);
	creatures.push_back(pumpkin);
	pumpkin->show();

	Chest* chest3 = new Chest(MIMIC);
	chest3->setParent(this);
	chest3->setDungeon(this);
	chest3->move(5 * 64, 43 * 64);
	objects.push_back(chest3);
	chest3->show();

	for (int i = 0; i < 5; i++)
	{
		OrcWarrior* orc = new OrcWarrior();
		orc->setParent(this);
		orc->setDungeon(this);
		orc->setRange(14 * 64, 26 * 64, 27 * 64, 21 * 64);
		orc->move(14 * 64 + ((rand() % 15 + 1) * 64), 26 * 64 + ((rand() % 15 + 1) * 64));
		creatures.push_back(orc);
		orc->show();
	}

	for (int i = 0; i < 10; i++)
	{
		Skull* skull = new Skull();
		skull->setParent(this);
		skull->setDungeon(this);
		skull->setRange(14 * 64, 26 * 64, 27 * 64, 21 * 64);
		skull->move(14 * 64 + ((rand() % 15 + 1) * 64), 26 * 64 + ((rand() % 15 + 1) * 64));
		creatures.push_back(skull);
		skull->show();
	}

	for (int i = 0; i < 3; i++)
	{
		Chort* chort = new Chort();
		chort->setParent(this);
		chort->setDungeon(this);
		chort->setRange(14 * 64, 26 * 64, 27 * 64, 21 * 64);
		chort->move(14 * 64 + ((rand() % 15 + 1) * 64), 26 * 64 + ((rand() % 15 + 1) * 64));
		creatures.push_back(chort);
		chort->show();
	}

	wizzard->raise();
	for (int i = 0; i < wizzard->getHeart().size(); i++)
	{
		wizzard->getHeart()[i]->raise();
		wizzard->getHeart()[i]->move(16 + i * 48, 745);
	}
}

void Dungeon::addFireball(int ax, int ay, int direction)
{
	FireBall* fb = new FireBall(ax, ay, direction);
	fb->setParent(this);
	fb->setDungeon(this);
	fb->setDamage(wizzard->getDamage());
	fb->show();
}

void Dungeon::generateMap()
{
	for (auto& i : wall)
		for (auto& j : i)
			delete(j);
	wall.clear();

	wall.resize(board.size() - 2);
	for (auto& i : wall)
		i.resize(board[0].size() - 2);

	vector<bool> wallUpDownFlag(board[0].size(), true);
	vector<bool> wallLeftRightFlag(board.size(), true);

	for (int i = 0; i < wall.size(); i++)
		for (int j = 0; j < wall[0].size(); j++)
		{
			if (board[i + 1][j + 1] && board[i + 1][j] && board[i + 1][j + 2] && wallUpDownFlag[j + 1])
			{
				wall[i][j] = new DungeonWall(WU);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();

				wallUpDownFlag[j + 1] = !wallUpDownFlag[j + 1];
			}
			else if (board[i + 1][j + 1] && board[i + 1][j] && board[i + 1][j + 2] && !wallUpDownFlag[j + 1])
			{
				wall[i][j] = new DungeonWall(WD);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();

				wallUpDownFlag[j + 1] = !wallUpDownFlag[j + 1];
			}
			else if (board[i + 1][j + 1] && board[i][j + 1] && board[i + 2][j + 1] && wallLeftRightFlag[i + 1])
			{
				wall[i][j] = new DungeonWall(WL);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();

				wallLeftRightFlag[i + 1] = !wallLeftRightFlag[i + 1];
			}
			else if (board[i + 1][j + 1] && board[i][j + 1] && board[i + 2][j + 1] && !wallLeftRightFlag[i + 1])
			{
				wall[i][j] = new DungeonWall(WR);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();

				wallLeftRightFlag[i + 1] = !wallLeftRightFlag[i + 1];
			}
		}

	for (int i = 0; i < wall.size(); i++)
		for (int j = 0; j < wall[0].size(); j++)
		{
			if (board[i + 1][j + 1] && board[i + 2][j + 1] && board[i + 1][j + 2] && wall[i][j + 1]->type == WU)
			{
				wall[i][j] = new DungeonWall(WCORINS_LU);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
			if (board[i + 1][j + 1] && board[i + 2][j + 1] && board[i + 1][j] && wall[i][j - 1]->type == WU)
			{
				wall[i][j] = new DungeonWall(WCORINS_RU);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
			if (board[i + 1][j + 1] && board[i][j + 1] && board[i + 1][j + 2] && wall[i][j + 1]->type == WU)
			{
				wall[i][j] = new DungeonWall(WU);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
			if (board[i + 1][j + 1] && board[i][j + 1] && board[i + 1][j] && wall[i][j - 1]->type == WU)
			{
				wall[i][j] = new DungeonWall(WU);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
			else if (board[i + 1][j + 1] && board[i + 2][j + 1] && board[i + 1][j + 2] && wall[i][j + 1]->type == WD)
			{
				wall[i][j] = new DungeonWall(WCOROUTS_L);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
			else if (board[i + 1][j + 1] && board[i][j + 1] && board[i + 1][j + 2] && wall[i][j + 1]->type == WD)
			{
				wall[i][j] = new DungeonWall(WCORINS_LD);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
			else if (board[i + 1][j + 1] && board[i + 2][j + 1] && board[i + 1][j] && wall[i][j - 1]->type == WD)
			{
				wall[i][j] = new DungeonWall(WCOROUTS_R);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
			else if (board[i + 1][j + 1] && board[i][j + 1] && board[i + 1][j] && wall[i][j - 1]->type == WD)
			{
				wall[i][j] = new DungeonWall(WCORINS_RD);
				wall[i][j]->move(64 + j * 64, 64 + i * 64);
				wall[i][j]->setParent(this);
				wall[i][j]->show();
			}
		}

	generateGround(1, 1);
}

void Dungeon::generateGround(int ax, int ay)
{
	wall[ay][ax] = new DungeonGround();
	wall[ay][ax]->move(64 + ax * 64, 64 + ay * 64);
	wall[ay][ax]->setParent(this);
	wall[ay][ax]->show();

	if (wall[ay + 1][ax] == nullptr) generateGround(ax, ay + 1);
	if (wall[ay - 1][ax] == nullptr) generateGround(ax, ay - 1);
	if (wall[ay][ax + 1] == nullptr) generateGround(ax + 1, ay);
	if (wall[ay][ax - 1] == nullptr) generateGround(ax - 1, ay);
}

bool Dungeon::isPosValid(int px, int py)
{
	if (wall[py / 64][px / 64] == nullptr || wall[py / 64 - 1][px / 64 - 1] == nullptr ||
		wall[py / 64][px / 64 - 1] == nullptr || wall[py / 64 - 1][px / 64] == nullptr) return false;

	if (!(wall[py / 64][px / 64]->type == GROUND && wall[py / 64 - 1][px / 64 - 1]->type == GROUND
		&& wall[py / 64][px / 64 - 1]->type == GROUND && wall[py / 64 - 1][px / 64]->type == GROUND)) return false;

	for (auto& creature : creatures)
		if ((px + 64 > creature->pos().x() && px < creature->pos().x() + 64) &&
			(py + 64 > creature->pos().y() - creature->getOverHeight() && py < creature->pos().y() + 64 - creature->getOverHeight()))
			return false;

	for (auto& object : objects)
		if ((px + 64 > object->pos().x() && px < object->pos().x() + 64) &&
			(py + 64 > object->pos().y() && py < object->pos().y() + 64))
			return false;

	return true;
}

bool Dungeon::isPosValidCreature(int px, int py)
{
	if (wall[py / 64][px / 64] == nullptr || wall[py / 64 - 1][px / 64 - 1] == nullptr ||
		wall[py / 64][px / 64 - 1] == nullptr || wall[py / 64 - 1][px / 64] == nullptr) return false;

	if (!(wall[py / 64][px / 64]->type == GROUND && wall[py / 64 - 1][px / 64 - 1]->type == GROUND
		&& wall[py / 64][px / 64 - 1]->type == GROUND && wall[py / 64 - 1][px / 64]->type == GROUND)) return false;

	if ((px + 64 > wizzard->pos().x() && px < wizzard->pos().x() + 64) &&
		(py + 64 > wizzard->pos().y() - wizzard->getOverHeight() && py < wizzard->pos().y() + 64 - wizzard->getOverHeight()))
		return false;

	return true;
}

void Dungeon::nextLevel()
{
	level++;
	for (auto& creature : creatures) creature->setId(-1);
	for (auto& object : objects) object->setId(-1);
	for (auto& peak : peaks) peak->setId(-1);
	camera->keyState[ESPACE] = true;

	if (level == 1) firstFloor();
	else if (level == 2) secondFloor();
	else if (level == 3) thirdFloor();
}