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
