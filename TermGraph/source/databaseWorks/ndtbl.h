#pragma once

#include "source/databaseWorks/tblbase.h"
#include "source/databaseWorks/dbtablenames.h"
#include "source/Model/Termin/nodeinfocontainer.h"
#include "source/Helpers/handytypes.h"

#include "source/databaseWorks/columns/tcolumn.h"

class NodeTable : public TblBase
{
public:
    enum LastEditSource {
        TakeFromNodeInfo = 0,
        AutoGenerate
    };

    NodeTable(QSqlDatabase* base): TblBase(base) { }
    ~NodeTable() override = default;

    QUuid nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const;

    QUuid addNode(const QString& name, const QUuid& groupUuid);
    QUuid addNode(const QUuid& uuid, const QString& name, const QUuid& groupUuid);
    QUuid addNode(const NodeInfoContainer& info);  // TODO: remove all other functioins and change to bool

    bool hasNodeWithUuid(const QUuid& uuid);

    UuidList getAllNodesUuids(const QUuid& groupUuid = QUuid());
    NodeInfoContainer getNode(const QUuid& uuid);
    NodeInfoContainer::List getAllNodesInfo(const QUuid& groupUuid);

    QDateTime getLastEdit(const QUuid& uuid);
    RecVector getAllLastEditRecords();

    bool updateNode(const NodeInfoContainer& info, LastEditSource lastEditSource, bool checkLastEdit = true);
    void deleteNode(const QUuid& uuid);

    const char* tableName() const override;
    TColumn primaryKey() const override;
    void initTable() override;

protected:
    TColumn::List getAllColumns() const override;

private:
    void setFieldUpdateLastEdit(const TColumn& column, const QUuid& uuid, const QString& val);
    void updateLastEdit(const QUuid &uuid);
    bool isUuidExist(const QUuid& uuid);

    QUuid generateNewUuid();

    static QDateTime getLastEditNow();
    static QString getLastEditNowString();

    QSqlRecord getNodeSqlRecord(const QUuid& uuid);  // TODO: Delete!

    NodeInfoContainer recordToNodeInfo(QSqlRecord& record);
};
