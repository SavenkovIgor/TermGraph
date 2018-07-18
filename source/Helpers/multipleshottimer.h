#ifndef MULTIPLESHOTTIMER_H
#define MULTIPLESHOTTIMER_H

#include <QTimer>

class MultipleShotTimer : public QTimer
{
    Q_OBJECT

    unsigned int shotsLimit;
    unsigned int shotsCount;

private slots:
    void increaseShot();

public:
    MultipleShotTimer(unsigned int shots, unsigned int interval);

};

#endif // MULTIPLESHOTTIMER_H
