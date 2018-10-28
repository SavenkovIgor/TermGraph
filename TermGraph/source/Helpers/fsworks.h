#ifndef FSWORKS_H
#define FSWORKS_H

#include <QApplication>
#include <QString>
#include <QDir>

class FSWorks
{
public:

    static void saveFile(const QString& subDir, const QString& fileName, QString data);
    static void saveFile(const QString& subDir, const QString& fileName, QByteArray data);
};

#endif // FSWORKS_H
