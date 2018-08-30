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
#include "mainscene.h"
#include "databaseWorks/dbabstract.h"
#include "mysceneview.h"
#include "wordfreqanalyze.h"
#include "Helpers/tagprocessor.h"

class MainScene;

class MainWindow : public QObject
{
    Q_OBJECT

private:
    DBAbstract *db;
    MainScene *scene;
    MyView *scView;
    Reminder *remind;
    WordFreqAnalyze* analyze;
    TagProcessor *tagProcessor;

public:
    explicit MainWindow(QObject *parent = 0);
    ~MainWindow();

    void saveSett();
    void loadSett();

private slots:
    void onQmlCreated(QObject *, QUrl);
    void openBase();

public slots:
    void takeSceneMouse(
            qreal x,
            qreal y,
            qreal winPosX,
            qreal winPosY,
            qreal hght,
            QString info = "",
            QString buttonInfo = "");

    void testSlot();

    int getUiElementSize(QString elementTypeName);

private:
    void initElemSizes();
    QMap<QString, int> elementSizes;

    QQmlApplicationEngine *engn;

    NetworkManager* network;
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;
};

#endif  // MAINWINDOW_H
