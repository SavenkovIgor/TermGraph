#ifndef TERMGROUPINFO_H
#define TERMGROUPINFO_H

#include <QDebug>
#include <QUuid>

#include "./Termin/graphicitemterm.h"

enum class GroupType {
    freeEdges = -1,
    terms = 0
};

class TermGroupInfo
{
public:
    TermGroupInfo();

    // Group info
    QUuid getUuid() const;
    GroupType getType() const;
    GraphicItemTerm::List getAllNodes() const;

protected:
    // Group info methods
    void setGroupUuid(QUuid uuid);
    void setType(const GroupType &value);

    // Group content methods
    void addNodeToList(GraphicItemTerm *node);
    void clearNodesList();

private:
    // Group info
    QUuid groupUuid;
    GroupType type = GroupType::freeEdges;

    // Group content
    GraphicItemTerm::List nodesList;
};

#endif // TERMGROUPINFO_H
