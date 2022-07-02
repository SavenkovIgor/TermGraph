// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDir>
#include <QStringList>
#include <QtGlobal>

// TODO: File rename
namespace AppSettings {

class StdPaths
{
public:
    static void createDefaultFoldersIfNeed();

    static QString backupFolder();
    static QString groupsJsonFolder();
    static QString defaultDatabaseFilePath();

private:
    static QString userAppConfigFolder();
};

} // namespace AppSettings
