#include "nodeverticalstack.h"

QSizeF NodeVerticalStackTools::getNodeVerticalStackedSize(const GraphicItemTerm::List& nodes)
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (GraphicItemTerm* node : nodes) {
        QSizeF sz = node->getFrameRect(CoordType::zeroPoint).size();
        height += sz.height();
        width = qMax(width, sz.width());
    }
    return QSizeF(width, height);
}

NodeVerticalStack::NodeVerticalStack()
{
    animationGroup.addAnimation(&swAnim1);
    animationGroup.addAnimation(&swAnim2);

    swAnim1.setPropertyName("pos");
    swAnim2.setPropertyName("pos");

    swAnim1.setEasingCurve(QEasingCurve::InOutQuad);
    swAnim2.setEasingCurve(QEasingCurve::InOutQuad);
}

NodeVerticalStack::~NodeVerticalStack()
{
    terms.clear();
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
    return NodeVerticalStackTools::getNodeVerticalStackedSize(terms);
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
    //sortTerms(); //TODO: Check. maybe causing random bug

    auto stackSize = getSize();
    QPointF startPoint(centerPoint.x(), centerPoint.y() - stackSize.height() / 2);

    auto placingPoint = startPoint;

    for (auto term : terms) {
        auto frameSize = term->getFrameRect(CoordType::zeroPoint).size();
        auto rectSize = term->getNodeRect(CoordType::zeroPoint).size();
        placingPoint.ry() += frameSize.height() / 2;

        QPointF leftTopPt;
        leftTopPt.setX(placingPoint.x() - rectSize.width() / 2);
        leftTopPt.setY(placingPoint.y() - rectSize.height() / 2);

        term->setPos(leftTopPt);

        placingPoint.ry() += frameSize.height() / 2;
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
