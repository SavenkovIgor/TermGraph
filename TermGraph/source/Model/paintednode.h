#ifndef PAINTEDNODE_H
#define PAINTEDNODE_H

#include <QSqlRecord>
#include <QSizeF>

#include "graphterm.h"
//#include "edge.h"

class PaintedNode : public GraphTerm
{
public:
    static bool someoneHover;
    static bool someoneSelect;

    PaintedNode(QSqlRecord rec);

    // Leveling tools
    int getUpLevels(int pLevel = -1);

protected:
    // Color tools
    QColor getBaseColor();
    QColor getSelectedColor();

//    EdgesList getEdgesInLayer();

    // Tools
    static bool isNearPoints(QPointF pt1, QPointF pt2, qreal dist);

    QRectF getInnerNodeRect() const;

    // Hovers
    bool thisHovered = false;
    bool thisSelected = false;

    QSizeF nodeSize = QSizeF(40.0, 10.0);

    // Paint / Animation
    qreal newPosOffs = 0.0;
};

#endif // PAINTEDNODE_H
