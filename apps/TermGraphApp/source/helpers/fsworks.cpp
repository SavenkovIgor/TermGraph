// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/fsworks.h"

#include <QCoreApplication>
#include <QDir>

QString FSWorks::workingDirPath() { return QDir().absolutePath(); }

bool FSWorks::createPath(const QString& path)
{
    auto dir = QDir();
    return dir.mkpath(path);
}

bool FSWorks::pathExist(const QString& path)
{
    auto dir = QDir(path);
    return dir.exists();
}

bool FSWorks::deletePath(const QString& path)
{
    auto dir = QDir(path);
    return dir.rmdir(path);
}

bool FSWorks::createFile(const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
        return true;
    }
    return false;
}

bool FSWorks::fileExist(const QString& filePath) { return QFile::exists(filePath); }

bool FSWorks::deleteFile(const QString& filePath) { return QFile::remove(filePath); }

void FSWorks::saveFile(const QString& subDir, const QString& fileName, const QString& data)
{
    saveFile(subDir, fileName, data.toUtf8());
}

void FSWorks::saveFile(const QString& subDir, const QString& fileName, const QByteArray& data)
{
    auto path = qApp->applicationDirPath() + "/" + subDir;
    createPath(path);
    QFile file(path + "/" + fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    }
}
