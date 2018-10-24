#ifndef TERMTREE_H
#define TERMTREE_H

#include <QList>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <stdlib.h>

#include "../Termin/graphicitemterm.h"
#include "../TerminEdge/edge.h"
#include "./nodeverticalstack.h"

class TermTree
{
public:
    typedef QList<TermTree*> List;

    TermTree();
    ~TermTree();

    QGraphicsRectItem* rect = nullptr;

    void setTreeNodeCoors(QPointF leftTopPoint = QPointF());

    void addTerm(GraphicItemTerm* term);
    bool hasTerm(GraphicItemTerm* term);
    bool hasEdge(Edge* edge);

    QSizeF getTreeSize() const;  // Clearly counted value. Ignoring real node positions

    TermTree(const TermTree&) = delete;
private:
    NodeVerticalStack::List stacks;

    GraphicItemTerm::List getAllNodesInTree() const;

    qreal getMaxStackHeight() const;
};

#endif // TERMTREE_H
