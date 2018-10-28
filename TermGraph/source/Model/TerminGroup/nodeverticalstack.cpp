#include "nodeverticalstack.h"

NodeVerticalStack::NodeVerticalStack()
{

}

void NodeVerticalStack::addTerm(GraphicItemTerm* term)
{
    terms << term;
}

bool NodeVerticalStack::hasTerm(GraphicItemTerm *term) const
{
    for (auto stackTerm : terms) {
        if (stackTerm == term) {
            return true;
        }
    }
    return false;
}

GraphicItemTerm::List NodeVerticalStack::getAllNodesInStack() const
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

void NodeVerticalStack::sortTerms()
{
    // Сначала сортируем по убыванию количества связей

    GraphicItemTerm::List tmpList = terms;
    int nMax = 0;

    for (int i = 0; i < tmpList.size(); i++) {
        nMax = i;
        for (int j = i + 1; j < tmpList.size(); j++) {
            if (tmpList[j]->getUpLevels() > tmpList[nMax]->getUpLevels()) {
                nMax = j;
            }
        }
        tmpList.swap(i, nMax);
    }

    GraphicItemTerm::List ret;
    // Формируем новый список так чтобы максимально нагруженные вершины были ближе к центру
    for (int i = 0; i < tmpList.size(); i++) {
        if (i%2 == 0)
            ret.push_front(tmpList[i]);
        else
            ret.push_back(tmpList[i]);
    }

    terms = ret;
}

void NodeVerticalStack::placeTerms(QPointF centerPoint)
{
    sortTerms();

    auto stackSize = getSize();
    QPointF startPoint(centerPoint.x(), centerPoint.y() + stackSize.height()/2);

    auto x = startPoint.x();
    auto y = startPoint.y();

    for (auto term : terms) {
        auto termSize = term->getFrameRect(CoordType::zeroPoint).size();
        y -= termSize.height()/2;
        QPointF leftTopPt(x - termSize.width()/2, y - termSize.height()/2);
        term->setPos(leftTopPt);
        y -= termSize.height()/2;
    }
}

void NodeVerticalStack::setNeighbours()
{
    // Casting list to GraphicItemTerm
    GraphTerm::List castedList;

    for (auto term: terms) {
        castedList << static_cast<GraphTerm*>(term);
    }

    for (auto term : terms) {
        term->clearNeighboursList();
        term->addLayerNeighbours(castedList);
    }
}

