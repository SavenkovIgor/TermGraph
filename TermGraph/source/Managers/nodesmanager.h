#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <QSqlRecord>
#include <QJsonObject>

#include "source/databaseWorks/database.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Managers/notificationmanager.h"

class NodesManager : public QObject
{
    Q_OBJECT

public:
    explicit NodesManager(QObject *parent = nullptr);

signals:
    void nodeChanged();

public slots:
    bool correctNewNodeName(const QString& name, QUuid& groupUuid, bool showWarnings = true);
    QUuid getNodeUuidByNameAndGroup(const QString& name, QUuid& groupUuid) const;
    bool hasNodeWithNameInGroup(const QString& name, QUuid& groupUuid) const;

    /*[[nodiscard]]*/ bool addNewNode(const QString& name,
                                      const QString& def,
                                      const QString& descr,
                                      const QString& exam,
                                      const QString& groupUuidString,
                                      const bool& sendChangeSignal = true);

    /*[[nodiscard]]*/ bool changeNode(const QUuid& nodeUuid,
                                      const QString& name,
                                      const QString& definition,
                                      const QString& description,
                                      const QString& example,
                                      const QString& groupUuidString,
                                      const bool& sendChangeSignal = true);

    /*[[nodiscard]]*/ bool changeNode(const QUuid& nodeUuid,
                                      const QString& name,
                                      const QString& definition,
                                      const QString& description,
                                      const QString& example,
                                      const QUuid& groupUuid,
                                      const bool& sendChangeSignal = true);
    void deleteNode(QUuid uuid);

    UuidList getAllNodesUuidsInGroup(const QUuid& groupUuid);
    PaintedTerm::List getAllNodesForGroup(const QUuid& groupUuid);

    QDateTime getLastEdit(QUuid nodeUuid);

    void importNodeFromJson(QJsonObject nodeJson, bool importIfGroupNotExist = false);
    QJsonObject getNodeJson(const QUuid& uuid);

private:
    bool correctGroupUuid(const QUuid& groupUuid, bool sendWarnings = true);
};
