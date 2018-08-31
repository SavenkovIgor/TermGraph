#include "fsworks.h"

void FSWorks::saveFile(QString subDir, QString fileName, QString data) {
    QByteArray arr;
    arr.append(data);
    saveFile(subDir, fileName, arr);
}

void FSWorks::saveFile(QString subDir, QString fileName, QByteArray data)
{
    QDir groupsDir(qApp->applicationDirPath());
    groupsDir.mkpath(subDir);
    QFile someFile(subDir + "/" + fileName);
    if (someFile.open(QIODevice::WriteOnly)) {
        someFile.write(data);
        someFile.close();
    }
}
