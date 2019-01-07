#ifndef TERMTREE_H
#define TERMTREE_H

#include <QList>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <stdlib.h>

#include "../Termin/graphicitemterm.h"
#include "../TerminEdge/edge.h"
#include "../GraphicItem/rectgraphicitem.h"
#include "./nodeverticalstack.h"

class TermTree
{
public:
    typedef QList<TermTree*> List;

    // Constructor/Destructor
    TermTree();
    ~TermTree();

    RectGraphicItem* rect = nullptr;

    // Add info
    void addTerm(GraphicItemTerm* term);

    // TreeInfo
    bool hasTerm(GraphicItemTerm* term) const;
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

    GraphicItemTerm* getNodeAtPoint(const QPointF &pt) const;

    void checkHover(QPointF mousePos);

    // Deletions
    TermTree(const TermTree&) = delete;

private:
    NodeVerticalStack::List stacks;

    // Internal Info
    GraphicItemTerm::List getAllNodesInTree() const;
    qreal getMaxStackHeight() const;
};

#endif // TERMTREE_H
