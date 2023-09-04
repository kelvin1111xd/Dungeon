#pragma once
#include <QApplication>
#include <QtWidgets/QWidget>
#include <QObject>
#include <QScrollArea>
#include <QScrollBar>
#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
#include <QMultiMedia>
#include <QMediaPlayer>
#include <QTimer>
#include <QVBoxLayout>
#include <vector>
#include <conio.h>
#include <ctime>
#include "Camera.h"
using namespace std;

class Wizzard;
class Object;
class Entity;

class Dungeon : public QWidget
{
private:
	Camera* camera = nullptr;
	QMediaPlayer* bgm = nullptr;
	QLabel* backGround = nullptr;
	Wizzard* wizzard = nullptr;
	vector<Entity*> creatures;
	vector<Object*> objects;
	vector<Entity*> peaks;
	QTimer* animation = nullptr;
	int worldFrame = 0;
	int level = 1;
	int resetColdDown = 0;
	vector<vector<bool>> board;
	vector<vector<Object*>> wall;

public:
	QApplication* app = nullptr;
	Dungeon();
	~Dungeon()
	{
		delete(backGround);
		for (int i = creatures.size() - 1; i >= 0; i--) delete(creatures[i]);
		for (int i = objects.size() - 1; i >= 0; i--) delete(objects[i]);
		for (int i = peaks.size() - 1; i >= 0; i--) delete(peaks[i]);
		delete(animation);
		for (int i = wall.size() - 1; i >= 0; i--)
			for (int j = wall[0].size() - 1; j >= 0; j--)
				delete(wall[i][j]);
		delete(camera);
	}

	void firstFloor();
	void secondFloor();
	void thirdFloor();
	void generateMap();
	void generateGround(int ax, int ay);
	bool isPosValid(int px, int py);
	bool isPosValidCreature(int px, int py);
	void addFireball(int ax, int ay, int direction);
	void nextLevel();

	void setLevel(int amount) { level = amount; }
	int getLevel() { return level; }
	Wizzard* getWizzard() { return wizzard; }
	vector<Entity*>& getCreatures() { return creatures; }
	vector<Object*>& getObjects() { return objects; }
	vector<Entity*>& getPeaks() { return peaks; }
	vector<vector<Object*>>& getWall() { return wall; }
};
