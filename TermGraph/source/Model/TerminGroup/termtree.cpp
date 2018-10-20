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

PaintedTerm::List TermTree::getAllNodesInTree() const
{
    PaintedTerm::List ret;
    for (auto stack : stacks) {
        ret << stack.getAllNodesInStack();
    }
    return ret;
}
