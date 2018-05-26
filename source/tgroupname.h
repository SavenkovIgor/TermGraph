#ifndef TGROUPNAME_H
#define TGROUPNAME_H

#include <QApplication>
#include <QObject>
#include <QFontMetricsF>
#include <QGraphicsSimpleTextItem>
#include "glb.h"

class TGroupName : public QGraphicsSimpleTextItem
{
public:
    TGroupName( QString name ):
        QGraphicsSimpleTextItem()
    {
        QPoint offset(10.0,Sizes::groupVerticalSpacer);
        QGraphicsSimpleTextItem::setPos( offset );

        this->name = name;
        updateName();
    }

    void timerStopped()
    {
        timerActive = false;
        updateName();
    }

    void timerStarted()
    {
        timerActive = true;
        updateName();
    }

    QString getNameOnly(){
        return name;
    }

    QRectF getNameRect() {
        return textMetric(getFullText());
    }

private:

    QString getFullText() const {
        return QGraphicsSimpleTextItem::text();
    }

    void updateName()
    {
        QString set = name;
        if( !timerActive )
            set += " (s)";
        setText( set );
    }

    QRectF textMetric(QString str) {
        QFontMetricsF mtr = QFontMetricsF( qApp->font() );
        return  mtr.boundingRect(str);
    }

    bool timerActive = true;
    QString name = "";
};

#endif // TGROUPNAME_H
