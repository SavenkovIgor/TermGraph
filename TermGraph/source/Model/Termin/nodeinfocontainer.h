#ifndef NODEINFOCONTAINER_H
#define NODEINFOCONTAINER_H

#include <vector>
#include <QString>
#include <QUuid>
#include <QDateTime>

struct NodeInfoContainer
{
    using List = std::vector<NodeInfoContainer>;

    QUuid uuid;
    QString name;
    QString wordForms;
    QString definition;
    QString description;
    QString examples;
    QString wikiRef;
    QString wikiImage;
    QUuid groupUuid;
    QDateTime lastEdit;
};

#endif // NODEINFOCONTAINER_H
