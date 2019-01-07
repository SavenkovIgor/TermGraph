#ifndef TGROUPNAME_H
#define TGROUPNAME_H

#include <QBrush>
#include <QGraphicsSimpleTextItem>

#include "../../Helpers/appstyle.h"
#include "../../Helpers/fonts.h"

class TGroupName : public QGraphicsSimpleTextItem
{
public:
    TGroupName(QString name):
        QGraphicsSimpleTextItem()
    {
        QPointF offset(10.0, AppStyle::Sizes::groupVerticalSpacer);
        QGraphicsSimpleTextItem::setPos(offset);

        this->setBrush(QBrush(AppStyle::Colors::Groups::text));
        this->setFont(Fonts::getWeightFont());
        this->name = name;
        setText(name);
    }

    QString getName() const
    {
        return name;
    }

    QSizeF getNameSize() {
        return Fonts::getTextMetrics(getName(), Fonts::getWeightFont());
    }

private:
    QString name = "";
};

#endif  // TGROUPNAME_H
