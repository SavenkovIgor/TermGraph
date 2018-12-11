#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <QObject>

#include "../glb.h"
#include "./nodesmanager.h"
#include "./networkmanager.h"
#include "../Model/TerminGroup/termgroup.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(
            NodesManager* nodesMgr,
            NetworkManager* network,
            QObject *parent = nullptr);

    QList<QUuid> getAllUuidsSortedByLastEdit();
    TermGroup* createGroup(const QUuid groupUuid);

signals:
    void groupsListChanged();

    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:
    QStringList getAllGroupsNames(bool withAllVeiw = false);
    QList <TermGroup*> getAllGroups();

    QString getGroupName(QUuid groupUuid);
    QUuid getGroupUuid(const QString &groupName);

    QStringList getGroupNames(const QList<QUuid> &groupUuids);

    void addNewGroup(const QString &name, const QString &comment);
    void deleteGroup(QString name);

    // Json
    void importGroupFromJsonFile(const QString& filename);
    void importGroupFromJson(const QString& rawJson);
    void importGroupFromJson(const QJsonDocument &json);

    // Network
    void sendGroupByNetwork(const QString groupName);

private:
    bool isValidGroupJson(const QJsonDocument json);

    TermGroup* createGroup(const QString &groupName);


    QDateTime getLastEdit(QUuid groupUuid);

    NodesManager* nodesMgr;
    NetworkManager* network;
};

#endif  // GROUPSMANAGER_H
