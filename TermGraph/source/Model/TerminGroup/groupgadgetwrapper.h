#pragma once

#include <QObject>

#include "source/Model/TerminGroup/groupinfocontainer.h"

class GroupGadgetWrapper
{
public:
    Q_GADGET

    Q_ENUM(GroupType)

    Q_PROPERTY(QString uuid READ uuid WRITE setUuid)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(GroupType type READ type WRITE setType)

    GroupGadgetWrapper(const GroupInfoContainer& info);

    // Uuid
    QString uuid() const;
    void setUuid(const QString& uuid);

    // Name
    QString name() const;
    void setName(const QString& name);

    // Comment
    QString comment() const;
    void setComment(const QString& comment);

    // Type
    GroupType type() const;
    void setType(const GroupType& type);

    GroupInfoContainer unwrap() const;

private:
    GroupInfoContainer _info;
};
