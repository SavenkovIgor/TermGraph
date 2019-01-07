#ifndef QUICKITEMTERM_H
#define QUICKITEMTERM_H

#include <QQuickPaintedItem>
#include <QSqlRecord>
#include "./paintedterm.h"

// This class contains function to draw Term in Qml scene
class QuickItemTerm final: public PaintedTerm, public QQuickPaintedItem
{
public:
    QuickItemTerm(QSqlRecord rec);

    // Overriding of QQuickPaintedItem
    void paint(QPainter *painter) override;

    // Overriding of PaintedTerm
    QPointF getPos() const override;
    QPointF getScenePos() const override;
    void movePosBy(qreal dx, qreal dy) override;
};

#endif // QUICKITEMTERM_H
