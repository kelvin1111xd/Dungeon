#include "Camera.h"
#include "Dungeon.h"
#include "Wizzard.h"
#include "Entity.h"
#include "Object.h"
#include "Direction.h"
#include "OrcWarrior.h"
#include "Chort.h"
#include "Potion.h"
#include "Chest.h"
#include "Mimic.h"
#include "Ladder.h"
#include "Skull.h"
#include "PumpkinDude.h"
#include "Peaks.h"

void Camera::intialize(Dungeon* dungeon)
{
	this->dungeon = dungeon;

	frame = new QTimer();
	frame->setInterval(50);
	QObject::connect(frame, &QTimer::timeout, [&]()
		{
			keyUpdate();
			if (coldDown != 0) coldDown--;
			if (eColdDown > 0) eColdDown--;
		});
	frame->start();
}

void Camera::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_W:
		keyState[EW] = true;
		break;
	case Qt::Key_S:
		keyState[ES] = true;
		break;
	case Qt::Key_A:
		keyState[EA] = true;
		break;
	case Qt::Key_D:
		keyState[ED] = true;
		break;
	case Qt::Key_E:
		keyState[EE] = true;
		break;
	case Qt::Key_Up:
		keyState[EUP] = true;
		break;
	case Qt::Key_Down:
		keyState[EDOWN] = true;
		break;
	case Qt::Key_Left:
		keyState[ELEFT] = true;
		break;
	case Qt::Key_Right:
		keyState[ERIGHT] = true;
		break;
	case Qt::Key_1:
		keyState[ESAVE] = true;
		save();
		break;
	case Qt::Key_2:
		keyState[ELOAD] = true;
		load();
		break;
	case Qt::Key_Space:
		keyState[ESPACE] = true;
		break;
	default:
		break;
	}

	for(int i = EW; i <= ED; i++)
		if (keyState[i])
		{
			dungeon->getWizzard()->setCurrentStatus(RUN);
			break;
		}
}

void Camera::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_W:
		keyState[EW] = false;
		break;
	case Qt::Key_S:
		keyState[ES] = false;
		break;
	case Qt::Key_A:
		keyState[EA] = false;
		break;
	case Qt::Key_D:
		keyState[ED] = false;
		break;
	case Qt::Key_E:
		keyState[EE] = false;
		break;
	case Qt::Key_Up:
		keyState[EUP] = false;
		break;
	case Qt::Key_Down:
		keyState[EDOWN] = false;
		break;
	case Qt::Key_Left:
		keyState[ELEFT] = false;
		break;
	case Qt::Key_Right:
		keyState[ERIGHT] = false;
		break;
	case Qt::Key_1:
		keyState[ESAVE] = false;
		break;
	case Qt::Key_2:
		keyState[ELOAD] = false;
		break;
	case Qt::Key_Space:
		keyState[ESPACE] = false;
		break;
	default:
		break;
	}

	dungeon->getWizzard()->setCurrentStatus(IDLE);
	for (int i = EW; i <= ED; i++)
		if (keyState[i])
		{
			dungeon->getWizzard()->setCurrentStatus(RUN);
			break;
		}
}

bool Camera::isScrollValid(int direction)
{
	QPoint wizzardPos = dungeon->getWizzard()->pos();

	if ((direction == EA || direction == ED) && wizzardPos.x() > 368 && wizzardPos.x() < 3408) return true;
	if ((direction == EW || direction == ES) && wizzardPos.y() > 320 && wizzardPos.y() < 3360) return true;

	return false;
}

void Camera::keyUpdate()
{
	Wizzard* wizzard = dungeon->getWizzard();
	if (wizzard->getHp() <= 0) return;

	for (int i = 0; i < EINVALID; i++)
		if (keyState[i])
		{
			switch (i)
			{
			case EW:
				if(wizzard->tryMove(0, -wizzard->getMoveSpeed()))
					if (isScrollValid(i))
					{
						verticalScrollBar()->setValue(verticalScrollBar()->value() - 8);
						for (auto& heart : wizzard->getHeart())
							heart->move(heart->pos().x(), heart->pos().y() - 8);
					}
				break;
			case ES:
				if(wizzard->tryMove(0, wizzard->getMoveSpeed()))
					if (isScrollValid(i))
					{
						verticalScrollBar()->setValue(verticalScrollBar()->value() + 8);
						for (auto& heart : wizzard->getHeart())
							heart->move(heart->pos().x(), heart->pos().y() + 8);
					}
				break;
			case EA:
				wizzard->setLook(LLEFT);
				if(wizzard->tryMove(-wizzard->getMoveSpeed(), 0))
					if (isScrollValid(i))
					{
						horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 8);
						for (auto& heart : wizzard->getHeart())
							heart->move(heart->pos().x() - 8, heart->pos().y());
					}
				break;
			case ED:
				wizzard->setLook(LRIGHT);
				if(wizzard->tryMove(wizzard->getMoveSpeed(), 0))
					if (isScrollValid(i))
					{
						horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 8);
						for (auto& heart : wizzard->getHeart())
							heart->move(heart->pos().x() + 8, heart->pos().y());
					}
				break;
			default:
				break;
			}
		}

	if (keyState[EE] == true && eColdDown == 0)
	{
		for (auto& object : dungeon->getObjects())
		{
			if (object->trigger)
			{
				object->triggerEvent();
				eColdDown = 10;
				break;
			}
		}
	}

	bool attackFlag = false;
	for (int i = EUP; i <= ERIGHT; i++)
		if (keyState[i]) attackFlag = true;

	if (attackFlag && coldDown == 0)
	{
		coldDown = 8;

		if (keyState[EUP] && keyState[ERIGHT])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DRU);
		}
		else if (keyState[EUP] && keyState[ELEFT])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DLU);
		}
		else if (keyState[EDOWN] && keyState[ELEFT])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DLD);
		}
		else if (keyState[EDOWN] && keyState[ERIGHT])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DRD);
		}
		else if (keyState[EUP])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DU);
		}
		else if (keyState[ELEFT])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DL);
		}
		else if (keyState[EDOWN])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DD);
		}
		else if (keyState[ERIGHT])
		{
			dungeon->addFireball(wizzard->pos().x(), wizzard->pos().y() - wizzard->getOverHeight(), DR);
		}
	}

	if (keyState[ESPACE])
	{
		QPoint wizzardPos = dungeon->getWizzard()->pos();
		if (wizzardPos.y() > 320 && wizzardPos.y() < 3360)
		{
			verticalScrollBar()->setValue(wizzardPos.y() - dungeon->getWizzard()->getOverHeight() + 32 - 400);
		}
		else
		{
			if (wizzardPos.y() <= 320)
				verticalScrollBar()->setValue(0);
			else
				verticalScrollBar()->setValue(3040);
		}
		if (wizzardPos.x() > 368 && wizzardPos.x() < 3408)
		{
			horizontalScrollBar()->setValue(wizzardPos.x() + 32 - 400);
		}
		else
		{
			if (wizzardPos.y() <= 368)
				horizontalScrollBar()->setValue(0);
			else
				horizontalScrollBar()->setValue(3040);
		}
		keyState[ESPACE] = false;
	}
}

void Camera::save()
{
	ofstream file;
	file.open("check_point.txt");
	file << dungeon->getLevel() << endl;
	file << dungeon->getWizzard()->pos().x() << " " << dungeon->getWizzard()->pos().y() << " " <<
		dungeon->getWizzard()->getHp() << " " << dungeon->getWizzard()->getDamage() << " " 
		<< dungeon->getWizzard()->getLook() << " " << 
		horizontalScrollBar()->value() << " " << verticalScrollBar()->value() << endl;
	file << dungeon->getCreatures().size() << endl;
	for (auto creature : dungeon->getCreatures())
		file << creature->getWhatIsThis() << " " << creature->pos().x() << " " << creature->pos().y() << " " <<
		creature->getHp() << " " << creature->getDamage() << " " << creature->getLook() << " " <<
		creature->getRange().x << " " << creature->getRange().y << " " <<
		creature->getRange().width << " " << creature->getRange().length << endl;
	file << dungeon->getObjects().size() << endl;
	for (auto object : dungeon->getObjects())
		file << object->getWhatIsThis() << " " << object->type << " " << 
		object->pos().x() << " " << object->pos().y() << endl;
	file << dungeon->getPeaks().size() << endl;
	for (auto peak : dungeon->getPeaks())
		file << peak->pos().x() << " " << peak->pos().y() << endl;
	file.close();
}

void Camera::load()
{
	for (auto& creature : dungeon->getCreatures()) creature->setId(-1);
	for (auto& object : dungeon->getObjects()) object->setId(-1);
	for (auto& peak : dungeon->getPeaks()) peak->setId(-1);

	ifstream file;
	file.open("check_point.txt");

	keyState[ESPACE] = true;
	int level;
	file >> level;
	if (level != dungeon->getLevel())
	{
		dungeon->setLevel(level);
		if (level == 1) dungeon->firstFloor();
		else if (level == 2) dungeon->secondFloor();
		else if (level == 3) dungeon->thirdFloor(); 
		for (auto& creature : dungeon->getCreatures()) creature->setId(-1);
		for (auto& object : dungeon->getObjects()) object->setId(-1);
	}
	file.ignore();

	int posX, posY, hp, damage, look, scrollX, scrollY;
	file >> posX >> posY >> hp >> damage >> look >> scrollX >> scrollY;
	dungeon->getWizzard()->move(posX, posY);
	dungeon->getWizzard()->setHp(3);
	for (int i = 0; i < dungeon->getWizzard()->getHeart().size(); i++)
	{
		dungeon->getWizzard()->getHeart()[i]->fill();
		dungeon->getWizzard()->getHeart()[i]->move(scrollX + 16 + i * 48, scrollY + 745);
	}
	if (hp <= 2)
	{
		dungeon->getWizzard()->setHp(2);
		dungeon->getWizzard()->getHeart()[1]->drop();
	}
	if (hp <= 1)
	{
		dungeon->getWizzard()->setHp(1);
		dungeon->getWizzard()->getHeart()[0]->drop();
	}
	dungeon->getWizzard()->setHp(hp);
	file.ignore();

	string creatureType;
	int creaturesSize, rangeX, rangeY, rangeWidth, rangeLength;
	file >> creaturesSize;
	file.ignore();
	for (int i = 0; i < creaturesSize; i++)
	{
		Entity* creature;
		file >> creatureType >> posX >> posY >> hp >> damage >> look >> rangeX >> rangeY >> rangeWidth >> rangeLength;
		if (creatureType == "OrcWarrior")
			creature = new OrcWarrior();
		else if (creatureType == "Chort")
			creature = new Chort();
		else if (creatureType == "Mimic")
			creature = new Mimic();
		else if (creatureType == "Skull")
			creature = new Skull();
		else if (creatureType == "PumpkinDude")
			creature = new PumpkinDude();
		creature->setParent(dungeon);
		creature->setDungeon(dungeon);
		creature->move(posX, posY);
		creature->setHp(hp);
		creature->setDamage(damage);
		creature->setLook(look);
		creature->setRange(rangeX, rangeY, rangeWidth, rangeLength);
		creature->show();
		dungeon->getCreatures().push_back(creature);
		file.ignore();
	}

	string objectType;
	int objectsSize, type;
	file >> objectsSize;
	file.ignore();
	for (int i = 0; i < objectsSize; i++)
	{
		Object* object;
		file >> objectType >> type >> posX >> posY;
		if (objectType == "Potion")
			object = new Potion(type);
		else if (objectType == "Chest")
			object = new Chest(type);
		else if (objectType == "Ladder")
			object = new Ladder();
		object->setParent(dungeon);
		object->setDungeon(dungeon);
		object->move(posX, posY);
		object->show();
		dungeon->getObjects().push_back(object);
		file.ignore();
	}

	int peaksSize;
	file >> peaksSize;
	file.ignore();
	for (int i = 0; i < peaksSize; i++)
	{
		Peaks* peak = new Peaks();
		file >> posX >> posY;
		peak->setParent(dungeon);
		peak->setDungeon(dungeon);
		peak->move(posX, posY);
		peak->show();
		dungeon->getPeaks().push_back(peak);
		file.ignore();
	}
	dungeon->getWizzard()->raise();
	file.close();
}