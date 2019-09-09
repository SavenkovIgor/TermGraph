#include "fsworks.h"

#include <QApplication>
#include <QDir>

bool FSWorks::createPath(const QString& path)
{
    auto dir = QDir();
    return dir.mkpath(path);
}

bool FSWorks::fileExist(const QString& filePath)
{
    return QFile::exists(filePath);
}

void FSWorks::saveFile(const QString& subDir, const QString& fileName, const QString& data)
{
    QByteArray arr;
    arr.append(data);
    saveFile(subDir, fileName, arr);
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
