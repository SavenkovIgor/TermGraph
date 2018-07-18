#include "multipleshottimer.h"

MultipleShotTimer::MultipleShotTimer(unsigned int shots, unsigned int interval):
    QTimer()
{
    setSingleShot(false);
    setInterval(interval);
    shotsLimit = shots;
    shotsCount = 0;
    connect(this, SIGNAL(timeout()), this, SLOT(increaseShot()));
}

void MultipleShotTimer::increaseShot()
{
    shotsCount++;
    if(shotsCount >= shotsLimit)
        stop(); //Останавливаем таймер
}
