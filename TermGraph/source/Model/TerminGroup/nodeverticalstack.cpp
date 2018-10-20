#include "nodeverticalstack.h"

NodeVerticalStack::NodeVerticalStack()
{

}

void NodeVerticalStack::addTerm(PaintedTerm *term)
{
    terms << term;
}

bool NodeVerticalStack::hasTerm(PaintedTerm *term)
{
    for (auto stackTerm : terms) {
        if (stackTerm == term) {
            return true;
        }
    }
    return false;
}

bool NodeVerticalStack::hasEdge(Edge *edge)
{
    PaintedTerm* rootTerm = dynamic_cast<PaintedTerm*>(edge->getRoot());
    PaintedTerm* leafTerm = dynamic_cast<PaintedTerm*>(edge->getLeaf());
    return hasTerm(rootTerm) && hasTerm(leafTerm);
}

PaintedTerm::List NodeVerticalStack::getAllNodesInStack() const
{
    return terms;
}

QSizeF NodeVerticalStack::getSize() const
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (auto node : terms) {
        QSizeF nodeSize = node->getFrameRect(CoordType::zeroPoint).size();
        height += nodeSize.height();
        width = qMax(width, nodeSize.width());
    }

    return QSizeF(width, height);
}

