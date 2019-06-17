#ifndef NDTBL_H
#define NDTBL_H

#include <string>
#include "./tblbase.h"
#include "./dbtablenames.h"
#include "../Model/Termin/nodeinfocontainer.h"
#include "../Helpers/handytypes.h"

#include "./columns/tcolumn.h"

class NodeTable : public TblBase
{
public:
    NodeTable(QSqlDatabase* base): TblBase(TableName::NODES, base) { }
    ~NodeTable() override = default;

    QUuid nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const;

    QUuid addNode(const QString& name, const QUuid& groupUuid);
    QUuid addNode(const QUuid& uuid, const QString& name, const QUuid& groupUuid);
    QSqlRecord getNodeSqlRecord(const QUuid& uuid);

    bool isNodeWithUuidExist(const QUuid& uuid);

    UuidList getAllNodesUuids(const QUuid& groupUuid = QUuid());
    NodeInfoContainer::List getAllNodesInfo(const QUuid& groupUuid);

    QDateTime getLastEdit(const QUuid& uuid);
    RecVector getAllLastEditRecords();

    void setName(const QUuid& uuid, const QString& name);
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

    void initTable() override;

protected:
    TColumn::List getAllColumns() const override;

private:
    void setFieldUpdateLastEdit(const TColumn& column, const QUuid& uuid, const QString& val);
    void updateLastEdit(const QUuid &uuid);
    bool isUuidExist(const QUuid& uuid);

    QUuid generateNewUuid();
};

#endif  // NDTBL_H
