#include "termtree.h"

TermTree::TermTree()
{
//    qDebug() << "Create tree";
    rect = new QGraphicsRectItem();
    rect->setBrush(AppStyle::Colors::transparent);
    rect->setPen(AppStyle::Colors::transparent);
//    rect->setPen(AppStyle::Colors::testColor);

    animationGroup.addAnimation(&swAnim1);
    animationGroup.addAnimation(&swAnim2);

    swAnim1.setPropertyName("pos");
    swAnim2.setPropertyName("pos");
    swAnim1.setEasingCurve(QEasingCurve::InOutQuad);
    swAnim2.setEasingCurve(QEasingCurve::InOutQuad);
}

TermTree::~TermTree()
{
    for (auto child : rect->childItems()) {
        child->setParentItem(nullptr);
    }
    delete rect;

    for (auto stack : stacks) {
        delete stack;
    }
}

void TermTree::setTreeNodeCoors(QPointF leftTopPoint)
{
    if (getAllNodesInTree().isEmpty()) {
        return;
    }

    qreal allTreeHeight = getMaxStackHeight();

    auto startPoint = leftTopPoint;
    startPoint.ry() += allTreeHeight / 2;

    auto centerPoint = startPoint;

    for (auto stack : stacks) {
        auto stackSize = stack->getSize();
        centerPoint.rx() += stackSize.width()/2;
        stack->placeTerms(centerPoint);
        centerPoint.rx() += stackSize.width()/2 + AppStyle::Sizes::treeLayerHorizontalSpacer;
    }
}

void TermTree::setNeighbours()
{
    for (auto stack : stacks) {
        stack->setNeighbours();
    }
}

void TermTree::animateTree()
{
    return;  // TODO: Delete!
    //    if(animGrp.state() != QAbstractAnimation::Stopped)
    //        return;
    if (lockForce) {
        return;
    }

    for (GraphicItemTerm* node : getAllNodesInTree()) {
        if (animationGroup.state() != QAbstractAnimation::Stopped && node->getPaintLevel() == currAnimLevel) {
            continue;
        }
        node->countForces();
    }

    bool someMoved = false;
    for (GraphicItemTerm* node : getAllNodesInTree()) {
        if (animationGroup.state() != QAbstractAnimation::Stopped && node->getPaintLevel() == currAnimLevel) {
            continue;
        }
        if (node->applyMove() && !someMoved) {
            someMoved = true;
        }
    }

    if (!someMoved) {
        // stopAnimation();  // TODO: uncomment
        //        animTimer.stop();
    }
}

void TermTree::checkSwap()
{
    for (int layer = 1; layer < stacks.size(); layer++) {  // TODO: Обдумать этот момент i=1 because we need to ignore roots.
        GraphicItemTerm::List levLst = stacks[layer]->getAllNodesInStack();

        for (int j = 0; j < levLst.size() - 1; j++) {
            int inter = levLst[j]->getIntersections();

            QPointF pt1 = levLst[j]->getCenter(CoordType::scene);
            QPointF pt2 = levLst[j+1]->getCenter(CoordType::scene);

            levLst[j]->setSwap(pt2);
            levLst[j+1]->setSwap(pt1);

            qreal sum1 = levLst[j]->getSumEdgesLength(false) + levLst[j+1]->getSumEdgesLength(false);
            qreal sum2 = levLst[j]->getSumEdgesLength(true) + levLst[j+1]->getSumEdgesLength(true);

            int newIntersect = levLst[j]->getIntersections(true);

            levLst[j]->dropSwap();
            levLst[j+1]->dropSwap();

            if (levLst[j]->getNodeRect(CoordType::scene).intersects(levLst[j+1]->getNodeRect(CoordType::scene))) {
                continue;
            }

            if (newIntersect < inter) {
                stacks[layer]->swapNodes(levLst[j], levLst[j+1]);
                //                qDebug()<<"swapReason:intersections"<<t.elapsed();
                return;
            } else if (newIntersect <= inter && (sum2 < sum1 && sum1-sum2 > sum1*0.2)) {
                stacks[layer]->swapNodes(levLst[j], levLst[j+1]);
                //                qDebug()<<"swapReason:length"<<t.elapsed();
                return;
            }
        }
    }

    lockForce = false;

    //    qDebug()<<"noSwap"<<t.elapsed();
}

GraphicItemTerm *TermTree::getNodeAtPoint(const QPointF& pt) const
{
    for (auto node : getAllNodesInTree()) {
        if (node->getNodeRect(CoordType::scene).contains(pt)) {
            return node;
        }
    }

    return nullptr;
}

void TermTree::checkHover(QPointF mousePos)
{
    for (auto node : getAllNodesInTree()) {
        auto hovers = node->getNodeRect(CoordType::scene).contains(mousePos);
        node->setHover(hovers);
    }
}

void TermTree::addTerm(GraphicItemTerm* term)
{
    int paintLayer = term->getPaintLevel();

    if (paintLayer < 0) {
        qDebug() << "Biig problems with paint layer";
        return;
    }

    int increaseSizeCount = paintLayer - stacks.size() + 1;
    for (int i = 0; i < increaseSizeCount; i++) {
        stacks.push_back(new NodeVerticalStack());
    }

    GraphicItemTerm* grTerm = dynamic_cast<GraphicItemTerm*>(term);
    grTerm->setSceneParent(rect);

    stacks[paintLayer]->addTerm(term);
}

bool TermTree::hasTerm(GraphicItemTerm *term) const
{
    for (auto stack : stacks) {
        if (stack->hasTerm(term)) {
            return true;
        }
    }
    return false;
}

bool TermTree::hasEdge(Edge *edge) const
{
    auto* rootTerm = dynamic_cast<GraphicItemTerm*>(edge->getRoot());
    auto* leafTerm = dynamic_cast<GraphicItemTerm*>(edge->getLeaf());
    return hasTerm(rootTerm) && hasTerm(leafTerm);
}

QRectF TermTree::getTreeRect(CoordType inCoordinates) const
{
    QRectF ret = QRectF(QPointF(), getTreeSize());

    switch (inCoordinates) {
    case CoordType::zeroPoint: break;
    case CoordType::local:
        ret = ret.translated(rect->pos());
        break;
    case CoordType::scene:
        ret = ret.translated(rect->scenePos());
        break;
    }

    return ret;
}

QSizeF TermTree::getTreeSize() const
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (auto stack : stacks) {
        QSizeF stackSize = stack->getSize();
        width += stackSize.width();
        height = qMax(height, stackSize.height());
    }

    if (!stacks.empty()) {
        width += (stacks.size() - 1)*AppStyle::Sizes::treeLayerHorizontalSpacer;
    }

    return QSizeF(width, height);
}

GraphicItemTerm::List TermTree::getAllNodesInTree() const
{
    GraphicItemTerm::List ret;
    for (auto stack : stacks) {
        ret << stack->getAllNodesInStack();
    }
    return ret;
}

qreal TermTree::getMaxStackHeight() const
{
    qreal maxHeight = 0.0;
    for (auto stack : stacks) {
        QSizeF stackSize = stack->getSize();
        maxHeight = qMax(maxHeight, stackSize.height());
    }
    return maxHeight;
}
