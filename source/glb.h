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
    Glb();
    static Qt::Orientation ori;

    static bool isVertical();
    static bool isHorizontal();

    static int getLevDistance(const QString &src, const QString &dst);

    static QString ptToStr( int x, int y, QString divider = "x" );
    static QString ptToStr( QPoint  pt,   QString divider = "x" );
    static QString ptToStr( QPointF pt,   QString divider = "x" );

    static CloudServicesWrapper *cloud;
};

class Sizes {
public:
    static qreal groupVerticalSpacer;
};

#include "termnode.h"
#include "edge.h"
#include "databaseWorks/dbabstract.h"
#include "databaseWorks/cloudservices.h"
//#include "mainscene.h"
//#include "reminder.h"

#endif // GLB_H
