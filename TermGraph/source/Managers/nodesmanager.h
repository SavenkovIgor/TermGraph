#ifndef NODESMANAGER_H
#define NODESMANAGER_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QSqlRecord>
#include <QJsonObject>

#include "../glb.h"
#include "../databaseWorks/dbabstract.h"
#include "../Model/graphicitemterm.h"

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
            QString definition,
            QString description,
            QString example,
            QString groupName);

    void changeNode(
            QUuid nodeUuid,
            QString name,
            QString forms,
            QString definition,
            QString description,
            QString example,
            QUuid groupUuid);

    void deleteNode(QUuid uuid);

    QList<QUuid> getAllNodesUuidsInGroup(QUuid groupUuid);
    GraphicItemTerm::List getAllNodesForGroup(QUuid groupUuid);

    QDateTime getLastEdit(QUuid nodeUuid);

    void importNodeFromJson(QJsonObject nodeObject);
private:
    QSqlRecord getNodeSqlRecord(QUuid nodeUuid);
};

#endif  // NODESMANAGER_H
