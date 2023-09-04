#include "Claw.h"

Claw::Claw()
{
	texture =
	{
		{
			QPixmap(".\\Texture\\Entity\\claw_f0.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\claw_f1.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\claw_f2.png").scaled(64, 64, Qt::KeepAspectRatio),
			QPixmap(".\\Texture\\Entity\\claw_f3.png").scaled(64, 64, Qt::KeepAspectRatio),
		}
	};
	setPixmap(texture.idle[0]);
	resize(64, 64);
	resetPos();

	actionTime = new QTimer(this);
	actionTime->setInterval(50);
	QObject::connect(actionTime, &QTimer::timeout, [&]()
		{
			frame++;
			if (frame == 4) delete(this);

			updateFrame(frame);
		});
	actionTime->start();
}