#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <QObject>
#include "../glb.h"
#include "nodesmanager.h"
#include "networkmanager.h"
#include "../termgroup.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(NodesManager* nodesMgr, NetworkManager* network, QObject *parent = nullptr);

signals:
    void groupsListChanged();

public slots:
    QStringList getAllGroupsNames(bool withAllVeiw = false);
    QList<QUuid> getAllGroupsUuids();

    QSqlRecord getGroupSqlRecord(QUuid groupUuid);
    QString getGroupNameByUuid(QUuid groupUuid);

    void addNewGroup(QString name, QString comment, int type);
    void deleteGroup(QString name);

    //Json
    void importGroupFromJsonFile(QString filename);
    void importGroupFromJson(QString rawJson);
    void importGroupFromJson(QJsonDocument json);

    //Network
    void sendGroupByNetwork(QString groupName);
private:
    bool isValidGroupJson(QJsonDocument json);
    TermGroup* getGroupByNameForInnerUse(QString name);

    NodesManager* nodesMgr;
    NetworkManager* network;
};

#endif // GROUPSMANAGER_H
