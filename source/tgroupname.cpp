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
    QFontMetrics mtr( qApp->font() );
    return mtr.boundingRect( text() );
}



void TGroupName::updateName()
{
    QString set = name;
    if( !timerActive )
        set += " (s)";
    setText( set );
}
