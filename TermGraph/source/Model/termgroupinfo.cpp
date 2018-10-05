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
