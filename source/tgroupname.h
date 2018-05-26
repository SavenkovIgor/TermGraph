#ifndef TGROUPNAME_H
#define TGROUPNAME_H

#include <QApplication>
#include <QObject>
#include <QFontMetricsF>
#include <QGraphicsSimpleTextItem>

class TGroupName : public QGraphicsSimpleTextItem
{
public:
    TGroupName( QString name ):
        QGraphicsSimpleTextItem()
    {
        QGraphicsSimpleTextItem::setPos( QPointF(10.0,7.0) );

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

    void setPos(const QPointF &pos)
    {
        QPointF newPt = pos /*+ QPointF(10.0,7.0)*/;//Небольшое смещение для красоты
        //TODO: Избавиться от смещения и считать по другому!
        QGraphicsSimpleTextItem::setPos( newPt );
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
