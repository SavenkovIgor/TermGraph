#include "termgroupinfo.h"

TermGroupInfo::~TermGroupInfo()
{
    for (auto tree : trees) {
        delete tree;
    }

    for (auto node : getAllNodes()) {
        delete node;
    }

    for (auto edge : getAllEdges()) {
        delete edge;
    }
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

PaintedTerm::List TermGroupInfo::getAllNodes() const
{
    return nodesList;
}

EdgesList TermGroupInfo::getAllEdges() const
{
    return edgesList;
}

EdgesList TermGroupInfo::getAllEdgesForPainting() const
{
    EdgesList lst;
    lst << edgesList;
    for (auto node : getAllNodes()) {
        for (auto edge : node->getBrokenEdges()) {
            auto castedEdge = dynamic_cast<Edge*>(edge);
            lst << castedEdge;
        }
    }
    return lst;
}

PaintedTerm::List TermGroupInfo::getRootNodes() const
{
    return filterFromNodesList([] (PaintedTerm* node) { return node->isRoot(); });
}

PaintedTerm::List TermGroupInfo::getInTreeNodes() const
{
    return filterFromNodesList([] (PaintedTerm* node) { return node->isInTree(); });
}

PaintedTerm::List TermGroupInfo::getOrphanNodes() const
{
    return filterFromNodesList([] (PaintedTerm* node) { return node->isOrphan(); } );
}

Edge* TermGroupInfo::addNewEdge(PaintedTerm* rootNode, PaintedTerm* leafNode)
{
    auto edge = new Edge(rootNode, leafNode);
    rootNode->addEdgeRef(edge);
    leafNode->addEdgeRef(edge);
    return edge;
}

EdgesList TermGroupInfo::searchAllConnections()
{
    EdgesList ret;
    // Compare everything with everything
    for (auto node : nodesList) {
        for (const auto& tag : node->getDefinitionTags()) {
            if (auto foundNode = getRootNodeForTag(tag)) {
                if (node != foundNode) {
                    ret << addNewEdge(foundNode, node);
                }
            }
        }
    }

    return ret;
}

PaintedTerm *TermGroupInfo::getRootNodeForTag(const QString &tag)
{
    PaintedTerm* targetTerm = nullptr;

    for (auto node : nodesList) {

        for (auto& termName : node->getNameFormList()) {  // TODO: Переименовать при случае

            if (termName.size() == tag.size() && termName == tag) {
                return node;
            }

            if (TagProcessor::isTagCorrespondToTermName(termName, tag)) {  // TODO: Rework comparsion scheme!
                targetTerm = node;
            }
        }
    }

    return targetTerm;
}

void TermGroupInfo::loadEdges()
{
    if (getType() == GroupType::terms) {
        edgesList << searchAllConnections();
    }
}

void TermGroupInfo::removeCycles()
{
    // First find all edges to break
    for (auto node : nodesList) {
        node->getCycleEdge();
    }

    EdgesList brokeList;
    for (auto edge : edgesList) {
        if (edge->needBroke) {
            brokeList << edge;
        }
    }

    for (auto edge : brokeList) {
        edge->brokeEdge();
        edgesList.removeOne(edge);
    }
}

void TermGroupInfo::removeExceedEdges()
{
    // First find all edges to break
    for (auto node : nodesList) {
        node->checkForExceedEdges();
    }

    EdgesList brokeList;
    for (auto edge : edgesList) {
        if (edge->needCutOut) {
            brokeList << edge;
        }
    }

    for (auto edge : brokeList) {
        edge->cutOutFromSides();
        edgesList.removeOne(edge);
        delete edge;
    }
}

void TermGroupInfo::setLevels()
{
    // Set layer numbers
    for (auto node : getRootNodes()) {
        node->setLevel(0);
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
        auto tree = new TermTree();
        for (auto node : treeNodes) {
            if (node->getTreeId() == treeId) {
                tree->addTerm(node);
            }
        }
        trees.push_back(tree);
    }
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



QSizeF TermGroupInfo::getOrphansSize()
{
    QRectF orphansRc;
    for (auto node : getOrphanNodes()) {
        orphansRc = orphansRc.united(node ->getNodeRect(CoordType::scene));
    }
    return orphansRc.size();
}

PaintedTerm::List TermGroupInfo::filterFromNodesList(std::function<bool(PaintedTerm*)> filterCheck) const
{
    PaintedTerm::List ret;
    for (auto n : nodesList) {
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

void TermGroupInfo::addNodeToList(PaintedTerm *node)
{
    nodesList << node;
}

void TermGroupInfo::clearNodesList()
{
    nodesList.clear();
}
