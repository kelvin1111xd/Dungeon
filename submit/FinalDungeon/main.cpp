#include "Dungeon.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Dungeon dungeon;
	dungeon.app = &a;
	return a.exec();
}
