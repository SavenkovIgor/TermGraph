#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

#include "./Model/reminder.h"
#include "./mainscene.h"
#include "./databaseWorks/dbabstract.h"
#include "./Model/wordfreqanalyze.h"
#include "./Helpers/tagprocessor.h"
#include "./Helpers/appconfig.h"
#include "./glb.h"

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow();

    // App settings
//    void saveAppSettings();
//    void loadAppSettings();

public slots:
    int getUiElementSize(QString elementTypeName);

private:
    void initElemSizes();
    QMap<QString, int> elementSizes;

    MainScene* scene;

    QQmlApplicationEngine* engn;

    NetworkManager* network;
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;

    // Reminder* remind;
    WordFreqAnalyze* analyze;
    TagProcessor* tagProcessor;
};

#endif  // MAINWINDOW_H
