// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include <TermDataInterface/GroupData.h>

class GroupDataWrapper : public GroupData
{
    Q_GADGET

    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid FINAL)
    Q_PROPERTY(QString name MEMBER name FINAL)
    Q_PROPERTY(QString comment MEMBER comment FINAL)

public:
    GroupDataWrapper() = default;
    GroupDataWrapper(const GroupData& info)
        : GroupData(info)
    {}

    // Uuid
    QString getUuid() const { return uuid ? uuid->toString() : QString(); }
    void    setUuid(const QString& uuid) { this->uuid = GroupUuid::create(uuid); }
};

Q_DECLARE_METATYPE(GroupDataWrapper)
