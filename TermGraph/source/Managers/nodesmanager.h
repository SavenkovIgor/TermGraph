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

    void addNewNode(QString name,
            const QString forms,
            const QString def,
            const QString descr,
            const QString exam,
            const QString groupName);

    void changeNode(
            QUuid nodeUuid,
            QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            QString groupName);

    void deleteNode(QUuid uuid);

    QDateTime getLastEdit(QUuid nodeUuid);
};

#endif  // NODESMANAGER_H
