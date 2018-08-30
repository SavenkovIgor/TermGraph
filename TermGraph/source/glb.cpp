#include "glb.h"

quint16 NetworkSettings::listenPort = 46377;

CloudServicesWrapper*Glb::cloud = nullptr;
DBAbstract* Glb::db = nullptr;



QString Glb::ptToStr(int x, int y, QString divider) {
    return QString::number(x) + divider + QString::number(y);
}

QString Glb::ptToStr(QPoint pt, QString divider){
    return ptToStr(pt.x(), pt.y(), divider);
}

QString Glb::ptToStr(QPointF pt, QString divider){
    return ptToStr(pt.toPoint(), divider);
}

void Glb::saveFile(QString subDir, QString fileName, QString data) {
    QByteArray arr;
    arr.append(data);
    saveFile(subDir, fileName, arr);
}

void Glb::saveFile(QString subDir, QString fileName, QByteArray data)
{
    QDir groupsDir(qApp->applicationDirPath());
    groupsDir.mkpath(subDir);
    QFile someFile(subDir + "/" + fileName);
    if (someFile.open(QIODevice::WriteOnly)) {
        someFile.write(data);
        someFile.close();
    }
}

void StdFolderPaths::createDefaultFoldersIfNeed()
{
    QStringList necessaryDirs;

#if defined( Q_OS_WIN ) || defined( Q_OS_LINUX ) || defined( Q_OS_MACOS )
    necessaryDirs << userAppConfigFolder();
    necessaryDirs << groupsJsonFolder();
#endif

    for (QString path : necessaryDirs) {
        QDir().mkpath(path);
    }
}

QString StdFolderPaths::groupsJsonFolder()
{
    return userAppConfigFolder() + "/GroupsJson";
}

QString StdFolderPaths::defaultDatabaseFilePath()
{
    QString dbFilePath;

#if defined( Q_OS_WIN ) || defined( Q_OS_LINUX ) || defined( Q_OS_MACOS )
    dbFilePath = userAppConfigFolder() + "/tg.termGraph";
#endif

#if defined( Q_OS_ANDROID )
    dbFilePath = "tg.termGraph";
#endif

    return dbFilePath;
}

QString StdFolderPaths::userAppConfigFolder()
{
    return QDir::home().absolutePath() + "/.TermGraph";
}
