#ifndef GLB_H
#define GLB_H

#include <QApplication>
#include <QObject>
#include <QtMath>
#include <QtSql>
#include <QList>
#include <QTimer>
#include <QDebug>
#include <QColor>
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
//class MainScene;
class Edge;
class DBAbstract;
class CloudServicesWrapper;

typedef QList< TermNode* >  NodesList;
typedef QList< Edge* >      EdgesList;

class Glb
{
public:
    static DBAbstract* db;

    static int getLevDistance(const QString &src, const QString &dst);

    static QString ptToStr( int x, int y, QString divider = "x" );
    static QString ptToStr( QPoint  pt,   QString divider = "x" );
    static QString ptToStr( QPointF pt,   QString divider = "x" );

    static CloudServicesWrapper *cloud;

    static void saveFile(QString subDir,QString fileName, QString data);
    static void saveFile(QString subDir,QString fileName, QByteArray data);

};

class Fonts {
public:
    static QFont getFont();
    static QFont getWidthFont();
    static QRectF getTextMetrics(QString text, QFont font = getFont());
};

class StdFolderPaths {
public:
    static QString groupsJsonPath();
};

class Sizes {
public:
    static qreal groupVerticalSpacer;
    static qreal groupHorizontalSpacer;

    static qreal orphansVerticalSpacer;
    static qreal orphansHorizontalSpacer;

    static qreal treeLayerHorizontalSpacer;
};

class Colors {
public:
    // Nodes
    static QColor nodeText;
    static QColor nodeBorder;

    static QColor nodeLeaf;
    static QColor nodeRoot;
    static QColor nodeOrphan;

    static QColor nodeLeafSelected;
    static QColor nodeRootSelected;
    static QColor nodeOrphanSelected;

    static QColor groupBorder;
    static QColor groupText;
    static QColor groupBackgroundTerms;
    static QColor groupBackgroundFreeConnections;

    static QColor sceneBackground;

    static QColor transparent;
};

class NetworkSettings {
public:
    static quint16 listenPort;
};

#include "termnode.h"
#include "edge.h"
#include "databaseWorks/dbabstract.h"
#include "databaseWorks/cloudservices.h"

#endif // GLB_H
