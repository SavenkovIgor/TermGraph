#pragma once

#include <QString>
#include <QUuid>

enum class GroupType { freeEdges = -1, terms = 0 };

struct GroupInfoContainer
{
    QUuid uuid;
    QString name;
    QString comment;
    GroupType type = GroupType::terms;
};
