#pragma once

#include <QObject>

#include "source/Model/TerminGroup/groupinfocontainer.h"

class GroupGadgetWrapper
{
    Q_GADGET

public:
    Q_ENUM(GroupType)

    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString comment READ getComment WRITE setComment)
    Q_PROPERTY(GroupType type READ getType WRITE setType)

    GroupGadgetWrapper() = default;
    GroupGadgetWrapper(const GroupInfoContainer& info);

    // Uuid
    QString getUuid() const;
    void setUuid(const QString& uuid);

    // Name
    QString getName() const;
    void setName(const QString& name);

    // Comment
    QString getComment() const;
    void setComment(const QString& comment);

    // Type
    GroupType getType() const;
    void setType(const GroupType& type);

    GroupInfoContainer unwrap() const;

private:
    GroupInfoContainer _info;
};

Q_DECLARE_METATYPE(GroupGadgetWrapper)
