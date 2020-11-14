/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/Helpers/fsworks.h"

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
