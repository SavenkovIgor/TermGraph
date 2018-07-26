#ifndef TGROUPNAME_H
#define TGROUPNAME_H

#include <QApplication>
#include <QObject>
#include <QGraphicsSimpleTextItem>

#include "./Helpers/appstyle.h"
#include "./glb.h"  // TODO: delete. Possibly dont need

class TGroupName : public QGraphicsSimpleTextItem
{
public:
    TGroupName(QString name):
        QGraphicsSimpleTextItem()
    {
        QPoint offset(10.0, Sizes::groupVerticalSpacer);
        QGraphicsSimpleTextItem::setPos(offset);

        this->setBrush(AppStyle::Colors::Groups::text);
        this->setFont(Fonts::getWeightFont());
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
        return Fonts::getTextMetrics(getFullText(), Fonts::getWeightFont());
    }

private:
    QString getFullText() const {
        return QGraphicsSimpleTextItem::text();
    }

    void updateName()
    {
        QString set = name;
        if (!timerActive) {
            set += " (s)";
        }
        setText(set);
    }

    bool timerActive = true;
    QString name = "";
};

#endif  // TGROUPNAME_H
