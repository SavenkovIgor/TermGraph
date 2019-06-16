#ifndef NODEINFOCONTAINER_H
#define NODEINFOCONTAINER_H

#include <QString>
#include <QUuid>
#include <QDateTime>

struct NodeInfoContainer
{
    QUuid nodeUuid;
    QString name;
    QString wordForms;
    QString definition;
    QString description;
    QString example;
    QString wikiRef;
    QString wikiImage;
    QUuid groupUuid;
    QDateTime lastEdit;
};

#endif // NODEINFOCONTAINER_H
