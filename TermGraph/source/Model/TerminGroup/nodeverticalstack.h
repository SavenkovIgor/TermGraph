#ifndef NODEVERTICALSTACK_H
#define NODEVERTICALSTACK_H

#include <QList>

#include "../Termin/graphicitemterm.h"
#include "../TerminEdge/edge.h"

class NodeVerticalStackTools {
public:
    static QSizeF getNodeVerticalStackedSize(const GraphicItemTerm::List &nodes);
};

class NodeVerticalStack
{
public:
    typedef QList<NodeVerticalStack*> List;

    NodeVerticalStack();
    ~NodeVerticalStack();

    void addTerm(GraphicItemTerm* term);
    bool hasTerm(GraphicItemTerm* term) const;

    GraphicItemTerm::List getAllNodesInStack() const;

    QSizeF getSize() const;  // Clearly counted value. Ignoring real node positions

    void sortTerms();
    void placeTerms(QPointF centerPoint);
    void setNeighbours();

    // Animation
    QParallelAnimationGroup animationGroup;
    QPropertyAnimation swAnim1;
    QPropertyAnimation swAnim2;

    void swapNodes(GraphicItemTerm *n1, GraphicItemTerm *n2)
    {
        if (animationGroup.state() != QAbstractAnimation::Stopped) {
            return;  // Very important instruction.
        }
        // Because of lags, timer can still work, but animation will slow down and it cause
        // animation lags

        if (!hasTerm(n1) || !hasTerm(n2)) {
            return;
        }

        //currAnimLevel = n1->getPaintLayer();

        QRectF rc1 = n1->getNodeRect(CoordType::local);
        QRectF rc2 = n2->getNodeRect(CoordType::local);

        QPointF pos1, pos2;

        if (rc1.top() < rc2.top()) {
            pos1 = QPointF(n1->pos().x() + rc1.width()/2 - rc2.width()/2, n1->pos().y());
            pos2 = QPointF(n2->pos().x() + rc2.width()/2 - rc1.width()/2, n2->pos().y() - rc1.height() + rc2.height());
        } else {
            pos1 = QPointF(n1->pos().x() + rc1.width()/2-rc2.width()/2, n1->pos().y() - rc2.height() + rc1.height());
            pos2 = QPointF(n2->pos().x() + rc2.width()/2-rc1.width()/2, n2->pos().y());
        }

        swAnim1.setTargetObject(n1);
        swAnim1.setStartValue(n1->pos());
        swAnim1.setEndValue(pos2);

        swAnim2.setTargetObject(n2);
        swAnim2.setStartValue(n2->pos());
        swAnim2.setEndValue(pos1);

        animationGroup.start();
    }

private:
    GraphicItemTerm::List terms;
};

#endif // NODEVERTICALSTACK_H
