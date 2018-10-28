#include "termtree.h"

TermTree::TermTree()
{
//    qDebug() << "Create tree";
    rect = new QGraphicsRectItem();
    rect->setBrush(AppStyle::Colors::transparent);
    rect->setPen(AppStyle::Colors::transparent);
    rect->setPen(AppStyle::Colors::testColor);
}

TermTree::~TermTree()
{
//    qDebug() << "Delete tree";
    for (auto stack : stacks) {
        delete stack;
    }
    delete rect;
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

void TermTree::addTerm(GraphicItemTerm* term)
{
    int paintLayer = term->getPaintLayer();

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
    GraphicItemTerm* rootTerm = dynamic_cast<GraphicItemTerm*>(edge->getRoot());
    GraphicItemTerm* leafTerm = dynamic_cast<GraphicItemTerm*>(edge->getLeaf());
    return hasTerm(rootTerm) && hasTerm(leafTerm);
}

QSizeF TermTree::getTreeSize() const
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (int i = 0; i < stacks.size(); i++) {
        QSizeF stackSize = stacks[i]->getSize();

        width += stackSize.width();

        if (i < stacks.size() - 1) {
            width += AppStyle::Sizes::treeLayerHorizontalSpacer;
        }

        height = qMax(height, stackSize.height());
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
