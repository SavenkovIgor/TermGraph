#ifndef NDTBL_H
#define NDTBL_H

#include <string>
#include "./tblbase.h"

using namespace std;

class NodeColumn {
public:
    // TODO: Грохнуть uid
    constexpr static auto uid         = TColumn("uid",         "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
    constexpr static auto longUID     = TColumn("longUID",     "TEXT");
    constexpr static auto term        = TColumn("term",        "TEXT");
    constexpr static auto termForms   = TColumn("termForms",   "TEXT");
    constexpr static auto definition  = TColumn("definition",  "TEXT");
    constexpr static auto description = TColumn("description", "TEXT");
    constexpr static auto examples    = TColumn("examples",    "TEXT");
    constexpr static auto wikiRef     = TColumn("wikiRef",     "TEXT");
    constexpr static auto wikiImg     = TColumn("wikiImg",     "TEXT");
    constexpr static auto termGroup   = TColumn("termGroup",   "INTEGER DEFAULT '-1'");
    constexpr static auto lastRemind  = TColumn("lastRemind",  "TEXT");
    constexpr static auto remindNum   = TColumn("remindNum",   "INTEGER DEFAULT '0'");
    constexpr static auto atLearn     = TColumn("atLearn",     "INTEGER DEFAULT '0'");
    constexpr static auto lastEdit    = TColumn("lastEdit",    "TEXT");

    constexpr static TColumn columns[] = {
        uid,
        longUID,
        term,
        termForms,
        definition,
        description,
        examples,
        wikiRef,
        wikiImg,
        termGroup,
        lastRemind,
        remindNum,
        atLearn,
        lastEdit
    };
};

class NodeTable : public TblBase
{
public:
    NodeTable(QSqlDatabase* base): TblBase("termNode", base) { }
    ~NodeTable() override = default;

    QUuid nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const;


    QUuid addNode(const QString &name, const QUuid& groupUuid);
    QUuid addNode(const QUuid &uuid, const QString &name, const QUuid& groupUuid);
    QSqlRecord getNodeSqlRecord(const QUuid& uuid);

    bool isNodeWithUuidExist(const QUuid& uuid);

    QList<QUuid> getAllNodesUuids();
    QList<QUuid> getAllNodesUuidsInGroup(const QUuid &groupUuid);

    QDateTime getLastEdit(const QUuid& uuid);

    void setName(const QUuid& uuid, const QString& name);
    void setWordForms(const QUuid& uuid, const QString& forms);
    void setDefinition(const QUuid& uuid, const QString& definition);
    void setDescription(const QUuid& uuid, const QString& description);
    void setExamples(const QUuid& uuid, const QString& example);
    void setWikiRef(const QUuid& uuid, const QString& wikiRef);
    void setWikiImg(const QUuid &uuid, const QString &wikiImage);
    void setAtLearn(const QUuid &uuid, const bool &learn);
    void setGroup(const QUuid &nodeUuid, const QUuid &groupUuid);

    int  getRemindNum(const QUuid& uuid);
    void setRemindNum(const QUuid& uuid, const int& num, const QDate& date);
//    void setRemindToday(int uid);

    void deleteNode(const QUuid& uuid);

protected:
    TColumn::List getAllColumns() const;

private:
    void setFieldUpdateLastEdit(const TColumn &column, const QUuid &uuid, const QString &val);
    void updateLastEdit(const QUuid &uuid);
    bool isUuidExist(const QUuid& uuid);

    QUuid generateNewUuid();
};

#endif  // NDTBL_H
