#ifndef GLB_H
#define GLB_H

#include <QApplication>
#include <QObject>
#include <QtMath>
#include <QtSql>
#include <QList>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QPushButton>
#include <QGraphicsItem>
#include <QGraphicsSceneHelpEvent>
#include <QQuickWidget>
#include <QQuickItem>
#include <QVariant>

#include <vector>
#include <algorithm>

class TermNode;
class Edge;
class DBAbstract;
class CloudServicesWrapper;

// TODO: Remove unneeded type
typedef QList<TermNode*> NodesList;
typedef QList<Edge*> EdgesList;

class Glb
{
public:
    static DBAbstract* db;

    static void saveFile(QString subDir, QString fileName, QString data);
    static void saveFile(QString subDir, QString fileName, QByteArray data);
};

class StdFolderPaths {
public:
    static void createDefaultFoldersIfNeed();

    static QString groupsJsonFolder();
    static QString defaultDatabaseFilePath();
private:
    static QString userAppConfigFolder();
};

class NetworkSettings {
public:
    static quint16 listenPort;
};

#include "./termnode.h"
#include "./edge.h"
#include "./databaseWorks/dbabstract.h"
#include "./databaseWorks/cloudservices.h"

#endif  // GLB_H
