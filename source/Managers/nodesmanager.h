#ifndef NODESMANAGER_H
#define NODESMANAGER_H

#include <QObject>
#include "../glb.h"

class NodesManager : public QObject
{
    Q_OBJECT
public:
    explicit NodesManager(QObject *parent = nullptr);

signals:
    void nodeChanged();

public slots:

    void addNewNode(
            QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            QString groupName );

    void changeNode(
            QUuid nodeUuid,
            QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            QString groupName );

    void deleteNode(QUuid uuid);
};

#endif // NODESMANAGER_H
