#include "termgroupinfo.h"

TermGroupInfo::TermGroupInfo()
{

}

QUuid TermGroupInfo::getUuid() const
{
    return groupUuid;
}

void TermGroupInfo::setGroupUuid(QUuid uuid)
{
    groupUuid = uuid;
}

GroupType TermGroupInfo::getType() const
{
    return type;
}

GraphicItemTerm::List TermGroupInfo::getAllNodes() const
{
    return nodesList;
}

EdgesList TermGroupInfo::getAllEdges() const
{
    return edgesList;
}

GraphicItemTerm::List TermGroupInfo::getRootNodes() const
{
    return filterFromNodesList([] (GraphicItemTerm* node) { return node->isRoot(); });
}

GraphicItemTerm::List TermGroupInfo::getInTreeNodes() const
{
    return filterFromNodesList([] (GraphicItemTerm* node) { return node->isInTree(); });
}

GraphicItemTerm::List TermGroupInfo::getOrphanNodes() const
{
    return filterFromNodesList([] (GraphicItemTerm* node) { return node->isOrphan(); } );
}

Edge* TermGroupInfo::addNewEdge(GraphicItemTerm* node1, GraphicItemTerm* node2)
{
    Edge* edge = new Edge(node1, node2);
    node1->addEdgeRef(edge);
    node2->addEdgeRef(edge);
    return edge;
}

EdgesList TermGroupInfo::searchConnections()
{
    EdgesList ret;
    // Compare everything with everything
    for (GraphicItemTerm* n : nodesList) {
        for (GraphicItemTerm* m : nodesList) {
            if (n == m) {
                continue;
            }

            for (QString tag : n->getTags()) {
                for (QString termName : m->getNameFormList()) {  // TODO: Переименовать при случае
                    if (TagProcessor::isTagCorrespondToTermName(termName, tag)) {
                        ret << addNewEdge(m, n);
                    }
                }
            }
        }
    }
    return ret;
}

void TermGroupInfo::loadEdges()
{
    if (getType() == GroupType::terms) {
        edgesList << searchConnections();
    }
}

void TermGroupInfo::setLayers()
{
    // Set layer numbers
    for (GraphicItemTerm* node : getRootNodes()) {
        node->setLevel(0);
    }

    // Set max layer variable
    for (GraphicItemTerm* node : nodesList) {
        layersCount = qMax(layersCount, node->getPaintLayer());
    }
}

void TermGroupInfo::initTrees()
{
    for (auto tree : trees) {
        delete tree;
    }
    trees.clear();
    unsigned int treeId = 1;

    auto treeNodes = getInTreeNodes();

    // Set all tree Id's
    for (auto node : treeNodes) {
        if (node->getTreeId() == 0) {
            node->setTreeId(treeId);
            treeId++;
        }
    }

    unsigned int treesCount = treeId - 1; // last treeId increase was fictious

    // Set all trees
    for (unsigned int treeId = 1; treeId <= treesCount; treeId++) {
        TermTree* tree = new TermTree();
        for (auto node : treeNodes) {
            if (node->getTreeId() == treeId) {
                tree->addTerm(node);
            }
        }
        trees.push_back(tree);
    }
}

QList<int> TermGroupInfo::getLayerNumbersList(bool withRoot) const
{
    QList<int> ret;
    int startFrom = 1;

    if (withRoot) {
        startFrom = 0;
    }

    for (int i = startFrom; i <= layersCount; i++) {
        ret << i;
    }

    return ret;
}

GraphicItemTerm::List TermGroupInfo::getNodesInLayer(int layer) const
{
    GraphicItemTerm::List ret;
    for (GraphicItemTerm* node : nodesList) {
        if (node->getPaintLayer() == layer) {
            ret << node;
        }
    }

    // sort
    int nMin;

    for (int i = 0; i < ret.size(); i++) {
        nMin = i;
        for (int j = i + 1; j < ret.size(); j++) {
            if (ret[j]->pos().y() < ret[nMin]->pos().y()) {
                nMin = j;
            }
        }
        ret.swap(i, nMin);
    }
    return ret;
}

QSizeF TermGroupInfo::getAllTreesSize()
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (auto tree : trees) {
        QSizeF treeSize = tree->getTreeSize();
        width = qMax(width, treeSize.width());
        height += treeSize.height();
    }

    if (!trees.isEmpty()) {
        height += (trees.size() - 1)*AppStyle::Sizes::groupVerticalSpacer;
    }

    return QSizeF(width, height);
}

void TermGroupInfo::setNeighbours()
{
    for (auto tree : trees) {
        tree->setNeighbours();
    }
}

QSizeF TermGroupInfo::getVerticalStackedSize(GraphicItemTerm::List lst) const
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (GraphicItemTerm* node : lst) {
        QSizeF sz = node->getFrameRect(CoordType::zeroPoint).size();
        height += sz.height();
        width = qMax(width, sz.width());
    }
    return QSizeF(width, height);
}

QSizeF TermGroupInfo::getOrphansSize()
{
    QRectF orphansRc;
    for (GraphicItemTerm* node : getOrphanNodes()) {
        orphansRc = orphansRc.united(node ->getNodeRect(CoordType::scene));
    }
    return orphansRc.size();
}

GraphicItemTerm::List TermGroupInfo::filterFromNodesList(std::function<bool(GraphicItemTerm*)> filterCheck) const
{
    GraphicItemTerm::List ret;
    for (GraphicItemTerm* n : nodesList) {
        if (filterCheck(n)) {
            ret << n;
        }
    }
    return ret;
}

void TermGroupInfo::setType(const GroupType &value)
{
    type = value;
}

void TermGroupInfo::addNodeToList(GraphicItemTerm *node)
{
    nodesList << node;
}

void TermGroupInfo::clearNodesList()
{
    nodesList.clear();
}
