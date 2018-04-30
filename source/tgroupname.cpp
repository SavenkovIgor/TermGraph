#include "tgroupname.h"

TGroupName::TGroupName( QString name ):
    QGraphicsSimpleTextItem()
{
    this->name = name;
    updateName();
}

void TGroupName::timerStopped()
{
    timerActive = false;
    updateName();
}

void TGroupName::timerStarted()
{
    timerActive = true;
    updateName();
}

QRect TGroupName::getGroupNameGeometry() {
    return textMetric( text() ).toRect();
}

void TGroupName::updateName()
{
    QString set = name;
    if( !timerActive )
        set += " (s)";
    setText( set );
}
