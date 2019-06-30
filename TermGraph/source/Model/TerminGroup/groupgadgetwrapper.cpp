#include "groupgadgetwrapper.h"

GroupGadgetWrapper::GroupGadgetWrapper(const GroupInfoContainer& info)
    : _info(info)
{}

QString GroupGadgetWrapper::getUuid() const
{
    return _info.uuid.toString();
}

void GroupGadgetWrapper::setUuid(const QString &uuid)
{
    _info.uuid.fromString(uuid);
}

QString GroupGadgetWrapper::getName() const
{
    return _info.name;
}

void GroupGadgetWrapper::setName(const QString& name)
{
    _info.name = name;
}

QString GroupGadgetWrapper::getComment() const
{
    return _info.comment;
}

void GroupGadgetWrapper::setComment(const QString& comment)
{
    _info.comment = comment;
}

GroupType GroupGadgetWrapper::getType() const
{
    return _info.type;
}

void GroupGadgetWrapper::setType(const GroupType& type)
{
    _info.type = type;
}

GroupInfoContainer GroupGadgetWrapper::unwrap() const
{
    return _info;
}
