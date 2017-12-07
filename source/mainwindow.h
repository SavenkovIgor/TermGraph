#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtQml>
#include <QFlags>
#include <QGraphicsScene>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QtQuickControls2/QQuickStyle>
#include <QQuickImageProvider>
#include <QQuickPaintedItem>
#include <QQmlApplicationEngine>

#include "reminder.h"
#include "termnode.h"
#include "termgroup.h"
#include "edge.h"
#include "mainscene.h"
#include "databaseWorks/dbabstract.h"
#include "mysceneview.h"
#include "wordfreqanalyze.h"
#include "glb.h"


class MainScene;
//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QObject//, public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QStringList groupsList READ groupsList WRITE setGroupsList NOTIFY groupsListChanged)
    Q_PROPERTY(int currGroupNum READ currGroupNum WRITE setCurrGroupNum NOTIFY currGroupNumChanged)
    Q_PROPERTY(QString changeNum READ changeNum WRITE setChangeNum NOTIFY changeNumChanged)

public:
private:
    DBAbstract *db;
    MainScene *scene;
    MyView *scView;
    Reminder *remind;
    WordFreqAnalyze* analyze;

    QString cubizeString(QString str);

public:
    explicit MainWindow(QObject *parent = 0);
    ~MainWindow();

    void saveSett();
    void loadSett();
    void loadDbPath();

private slots:
    void onQmlCreated(QObject *, QUrl );
public:

    int currGroupNum() const { return m_currGroupNum; }
    QString changeNum() const { return m_changeNum; }
    QStringList groupsList() const { return m_groupsList; }

private slots:
    void openBase();

    void addNewPoint();

    void updGroupAddNode(int num);

public slots:
    void viewGrp(int num);
    QStringList getGroupsList();

    void addNewNode(QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            bool    isRoot,
            QString groupName
            );

    void ctrlMove(QString str);  //в секцию public slots и не ебись больше с ней
    void takeSceneMouse(qreal x, qreal y, qreal winPosX, qreal winPosY, qreal hght, QString info = "", QString buttonInfo = "");

    void sceneChangePoint();

    void updateGroupLists();

    void scaleUp();
    void scaleDown();

    void testSlot();

    void setSceneSize(int width,int height);
    void setCurrGroupNum(int currGroupNum);

    void setChangeNum(QString changeNum);

    void setGroupsList(QStringList groupsList);

    QStringList getGroupTypes();

    int getUiElementSize( QString elementTypeName );
signals:
    void currGroupNumChanged(int currGroupNum);

    void changeNumChanged(QString changeNum);

    void groupsListChanged(QStringList groupsList);

private:
    void initElemSizes();
    QMap<QString,int> elementSizes;

    QQmlApplicationEngine *engn;

    QString dbPath;
    int m_currGroupNum;
    QString m_changeNum;
    QStringList m_groupsList;
};


#endif // MAINWINDOW_H
