#pragma once

#include <QString>
#include <QByteArray>

class FSWorks
{
public:
    static bool createPath(const QString& path);

    static bool fileExist(const QString& filePath);

    static void saveFile(const QString& subDir, const QString& fileName, const QString& data);
    static void saveFile(const QString& subDir, const QString& fileName, const QByteArray& data);
};
