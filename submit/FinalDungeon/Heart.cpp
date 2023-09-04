#include "Heart.h"

Heart::Heart()
{
	texture =
	{
		{
			QPixmap(".\\Texture\\Entity\\ui_heart_empty.png").scaled(39, 36, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\ui_heart_half.png").scaled(39, 36, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\ui_heart_half.png").scaled(39, 36, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\ui_heart_full.png").scaled(39, 36, Qt::KeepAspectRatio),
		}
	};
	currentStatus = IDLE;
	setPixmap(texture.idle[3]);
	resize(39, 36);
	resetPos();
}

void Heart::drop()
{
	if (frame != 3) return;

	actionTime = new QTimer();
	actionTime->setInterval(50);
	QObject::connect(actionTime, &QTimer::timeout, [&]() 
		{
			frame--;
			updateFrame(frame);
			if (frame == 0)
				delete(actionTime);
		});
	actionTime->start();
}

void Heart::fill()
{
	if (frame != 0) return;

	actionTime = new QTimer();
	actionTime->setInterval(50);
	QObject::connect(actionTime, &QTimer::timeout, [&]()
		{
			frame++;
			updateFrame(frame);
			if (frame == 3)
				delete(actionTime);
		});
	actionTime->start();
}