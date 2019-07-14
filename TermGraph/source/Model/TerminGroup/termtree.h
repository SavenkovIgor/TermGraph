#pragma once

#include <QList>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <stdlib.h>

#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/edge.h"
#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/TerminGroup/nodeverticalstack.h"

class TermTree
{
public:
    using List = QList<TermTree*>;

    // Constructor/Destructor
    TermTree();
    ~TermTree();

    RectGraphicItem* rect = nullptr;

    // Add info
    void addTerm(PaintedTerm* term);

    // TreeInfo
    bool hasTerm(PaintedTerm* term) const;
    bool hasEdge(Edge* edge) const;

    QRectF getTreeRect(CoordType inCoordinates) const;
    QSizeF getTreeSize() const;  // Clearly counted value. Ignoring real node positions

    // Internal counts and preparations
    void setTreeNodeCoors(QPointF leftTopPoint = QPointF());
    void setNeighbours();

    // Animation  // TODO: Make animation! Later...
    QParallelAnimationGroup animationGroup;
    QPropertyAnimation swAnim1;
    QPropertyAnimation swAnim2;

    int currAnimLevel = -1;
    bool lockForce = true;

    void animateTree();
    void checkSwap();

    PaintedTerm* getNodeAtPoint(const QPointF &pt) const;

    void checkHover(QPointF mousePos);

    // Deletions
    TermTree(const TermTree&) = delete;

private:
    NodeVerticalStack::List stacks;

    // Internal Info
    PaintedTerm::List getAllNodesInTree() const;
    qreal getMaxStackHeight() const;
};
