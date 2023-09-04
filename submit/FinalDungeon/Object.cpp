#include "Object.h"
#include "Dungeon.h"

void Object::generatePressHint()
{
	eHint = new QLabel();
	eHint->setPixmap(QPixmap("./Texture/e_hint.png"));
	eHint->resize(32, 32);
	eHint->setParent(dungeon);
	eHint->move(pos().x() + 16, pos().y() - 12);
	eHint->show();
}