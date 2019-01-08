#ifndef TERMNODE_H
#define TERMNODE_H

#include <QDebug>

#include "../../Helpers/appstyle.h"
#include "../../Helpers/helpstuff.h"
#include "../GraphicItem/graphicitem.h"
#include "./paintedterm.h"

// This class contains function to draw Term in particular system of drawing
class GraphicItemTerm final: public PaintedTerm, public GraphicItem
{
    Q_OBJECT

public:
    typedef QList<GraphicItemTerm*> List;  // TODO: Перенести частично в родительский элемент

    GraphicItemTerm(QSqlRecord rec);
    ~GraphicItemTerm() override;

    // Overriding of PaintedTerm
    QPointF getPos() const override;
    QPointF getScenePos() const override;
    void movePosBy(qreal dx, qreal dy) override;
};

#endif  // TERMNODE_H
