#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <QObject>
#include "../glb.h"
#include "nodesmanager.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(NodesManager* nodesMgr, QObject *parent = nullptr);

signals:
    void groupsListChanged();

public slots:
    QStringList getAllGroupsNames(bool withAllVeiw = false);
    QList<QUuid> getAllGroupsUuids();

    QSqlRecord getGroupSqlRecord(QUuid groupUuid);

    void addNewGroup(QString name, QString comment, int type);
    void deleteGroup(QString name);

    //Json
    void importGroupFromJsonFile(QString filename);
    void importGroupFromJson(QString rawJson);
    void importGroupFromJson(QJsonDocument json);

private:
    bool isValidGroupJson(QJsonDocument json);

    NodesManager* nodesMgr;
};

#endif // GROUPSMANAGER_H
