#include "fsworks.h"

bool FSWorks::fileExist(const QString &filePath)
{
    return QFile::exists(filePath);
}

void FSWorks::saveFile(const QString& subDir, const QString& fileName, const QString& data) {
    QByteArray arr;
    arr.append(data);
    saveFile(subDir, fileName, arr);
}

void FSWorks::saveFile(const QString& subDir, const QString& fileName, const QByteArray& data)
{
    QDir groupsDir(qApp->applicationDirPath());
    groupsDir.mkpath(subDir);
    QFile someFile(subDir + "/" + fileName);
    if (someFile.open(QIODevice::WriteOnly)) {
        someFile.write(data);
        someFile.close();
    }
}
