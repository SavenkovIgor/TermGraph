// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class TermGroup;

class QtTermGroup : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString uuid READ qmlUuid CONSTANT FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL)

public:
    QtTermGroup(const std::shared_ptr<TermGroup> termGroup, QObject* parent = nullptr);
    ~QtTermGroup() = default;

    std::shared_ptr<TermGroup> termGroup() const { return mTermGroup; }

private:
    QString name() const;
    QString qmlUuid() const;

private:
    std::shared_ptr<TermGroup> mTermGroup;
};
