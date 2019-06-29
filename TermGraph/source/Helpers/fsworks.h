#pragma once

#include <QApplication>
#include <QString>
#include <QDir>

class FSWorks
{
public:

    static bool fileExist(const QString& filePath);

    static void saveFile(const QString& subDir, const QString& fileName, const QString& data);
    static void saveFile(const QString& subDir, const QString& fileName, const QByteArray& data);
};
