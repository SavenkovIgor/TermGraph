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

    NodeTable(QSqlDatabase* base): TblBase(TableName::NODES, base) { }
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

    void setName(const QUuid& uuid, const QString& name); // TODO: Try to delete
    void setWordForms(const QUuid& uuid, const QString& forms);
    void setDefinition(const QUuid& uuid, const QString& definition);
    void setDescription(const QUuid& uuid, const QString& description);
    void setExamples(const QUuid& uuid, const QString& example);
    void setWikiRef(const QUuid& uuid, const QString& wikiRef);
    void setWikiImg(const QUuid& uuid, const QString& wikiImage);
    void setAtLearn(const QUuid& uuid, const bool& learn);
    void setGroup(const QUuid& nodeUuid, const QUuid& groupUuid);

    int  getRemindNum(const QUuid& uuid);
    void setRemindNum(const QUuid& uuid, const int& num, const QDate& date);
//    void setRemindToday(int uid);

    void deleteNode(const QUuid& uuid);

    const char* tableName() const override;
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
