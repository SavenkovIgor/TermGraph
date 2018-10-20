#include "termtree.h"

TermTree::TermTree()
{

}

void TermTree::addTerm(PaintedTerm *term)
{
    int paintLayer = term->getPaintLayer();

    if (paintLayer < 0) {
        qDebug() << "Biig problems with paint layer";
        return;
    }

    int increaseSizeCount = paintLayer - stacks.size() + 1;
    for (int i = 0; i < increaseSizeCount; i++) {
        stacks.push_back(NodeVerticalStack());
    }

    stacks[paintLayer].addTerm(term);
}

bool TermTree::hasTerm(PaintedTerm *term)
{
    for (auto stack : stacks) {
        if (stack.hasTerm(term)) {
            return true;
        }
    }
    return false;
}

void TermTree::setSceneParent(QGraphicsItem *parent)
{
    for (auto node : getAllNodesInTree()) {
        GraphicItemTerm* term = dynamic_cast<GraphicItemTerm*>(node);
        term->setSceneParent(parent);
    }
}

QSizeF TermTree::getTreeSize() const
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (int i = 0; i < stacks.size(); i++) {
        QSizeF stackSize = stacks[i].getSize();

        width += stackSize.width();

        if (i < stacks.size() - 1) {
            width += AppStyle::Sizes::treeLayerHorizontalSpacer;
        }

        height = qMax(height, stackSize.height());
    }
    return QSizeF(width, height);
}

PaintedTerm::List TermTree::getAllNodesInTree() const
{
    PaintedTerm::List ret;
    for (auto stack : stacks) {
        ret << stack.getAllNodesInStack();
    }
    return ret;
}
