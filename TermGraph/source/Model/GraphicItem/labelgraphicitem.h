#ifndef LABELGRAPHICITEM_H
#define LABELGRAPHICITEM_H

#include <QString>

#include "../../Helpers/fonts.h"
#include "./graphicitem.h"

class LabelGraphicItem : public GraphicItem
{
public:
    LabelGraphicItem();
    LabelGraphicItem(const QString& label, const QFont& font = Fonts::getWeightFont());
    ~LabelGraphicItem() override;

    void setLabel(const QString& label);
    void setFont(const QFont& font);

    QString getLabel() const;
    QSizeF getNameSize() const;

private:
    QString _label;
    QFont _font;
};

#endif // LABELGRAPHICITEM_H
