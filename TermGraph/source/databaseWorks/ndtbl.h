#ifndef NDTBL_H
#define NDTBL_H

#include <string>
#include "./tblbase.h"

using namespace std;

class NodeColumn {
public:
    constexpr static auto uid         = "uid";  // TODO: Грохнуть
    constexpr static auto longUID     = "longUID";
    constexpr static auto term        = "term";
    constexpr static auto termForms   = "termForms";
    constexpr static auto definition  = "definition";
    constexpr static auto description = "description";
    constexpr static auto examples    = "examples";
    constexpr static auto wikiRef     = "wikiRef";
    constexpr static auto wikiImg     = "wikiImg";
    constexpr static auto termGroup   = "termGroup";
    constexpr static auto lastRemind  = "lastRemind";
    constexpr static auto remindNum   = "remindNum";
    constexpr static auto atLearn     = "atLearn";
    constexpr static auto lastEdit    = "lastEdit";
};

class NodeTable : public TblBase
{
public:
    NodeTable(QSqlDatabase* base):
        TblBase("termNode", base)
    {
        initColumn(NodeColumn::uid,         "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        initColumn(NodeColumn::longUID,     "TEXT");
        initColumn(NodeColumn::term,        "TEXT");
        initColumn(NodeColumn::termForms,   "TEXT");
        initColumn(NodeColumn::definition,  "TEXT");
        initColumn(NodeColumn::description, "TEXT");
        initColumn(NodeColumn::examples,    "TEXT");
        initColumn(NodeColumn::wikiRef,     "TEXT");
        initColumn(NodeColumn::wikiImg,     "TEXT");
        initColumn(NodeColumn::termGroup,   "INTEGER DEFAULT '-1'");  // -1 - because uid [0;+8)
        initColumn(NodeColumn::lastRemind,  "TEXT");
        initColumn(NodeColumn::remindNum,   "INTEGER DEFAULT '0'");
        initColumn(NodeColumn::atLearn,     "INTEGER DEFAULT '0'");
        initColumn(NodeColumn::lastEdit,    "TEXT");
    }

    bool hasNodeWithNameInGroup(const QString& name, const QUuid& groupUuid) const;

    QUuid addNode(const QString &name);
    QUuid addNode(const QUuid &uuid, const QString &name);
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

private:
    void setFieldUpdateLastEdit(const QString &columnName, const QUuid &uuid, const QString &val);
    void updateLastEdit(const QUuid &uuid);
    bool isUuidExist(const QUuid& uuid);

    QUuid generateNewUuid();
};

#endif  // NDTBL_H
