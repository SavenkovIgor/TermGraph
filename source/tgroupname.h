#ifndef TGROUPNAME_H
#define TGROUPNAME_H

#include <QApplication>
#include <QObject>
#include <QFontMetricsF>
#include <QGraphicsSimpleTextItem>

class TGroupName : public QGraphicsSimpleTextItem
{
    QRectF textMetric(QString str) {
        QFontMetricsF mtr = QFontMetricsF( qApp->font() );
        return  mtr.boundingRect(str);
    }

public:
    TGroupName( QString name );

    void timerStopped();
    void timerStarted();

    QRect   getGroupNameGeometry();

    QString getName(){
        return name;
    }

    qreal getNameWidth() {
        return textMetric(name).width();
    }

    void setPos(const QPointF &pos)
    {
        QPointF newPt = pos /*+ QPointF(10.0,7.0)*/;//Небольшое смещение для красоты
        //TODO: Избавиться от смещения и считать по другому!
        QGraphicsSimpleTextItem::setPos( newPt );
    }

private:

    QString text() const {
        return QGraphicsSimpleTextItem::text();
    }

    void updateName();
    bool timerActive = true;
    QString name = "";
};

#endif // TGROUPNAME_H
