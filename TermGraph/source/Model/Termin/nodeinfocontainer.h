#pragma once

#include <vector>
#include <QString>
#include <QUuid>
#include <QDateTime>

struct NodeInfoContainer
{
    using List = std::vector<NodeInfoContainer>;

    QUuid uuid;
    QString term;
    QString definition;
    QString description;
    QString examples;
    QString wikiUrl;
    QString wikiImage;
    QUuid groupUuid;
    QDateTime lastEdit;
};
