// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QByteArray>
#include <QString>

class FSWorks
{
public:
    // Create|Read|Delete
    static QString workingDirPath();
    static bool    createPath(const QString& path);
    static bool    pathExist(const QString& path);
    static bool    deletePath(const QString& path);

    // Create|Read|Delete
    static bool createFile(const QString& filePath);
    static bool fileExist(const QString& filePath);
    static bool deleteFile(const QString& filePath);

    static void saveFile(const QString& subDir, const QString& fileName, const QString& data);
    static void saveFile(const QString& subDir, const QString& fileName, const QByteArray& data);
};
