#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

#include "./Model/reminder.h"
#include "./mainscene.h"
#include "./paintqueuemanager.h"
#include "./databaseWorks/dbabstract.h"
#include "./Model/wordfreqanalyze.h"
#include "./Helpers/tagprocessor.h"
#include "./Helpers/textprocessor.h"
#include "./Helpers/appconfig.h"
#include "./glb.h"

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow();

public slots:
    int getUiElementSize(const QString& elementTypeName);
    int dbVersion();

private:
    void initElemSizes();
    QMap<QString, int> elementSizes;

    // Independent init
    QScopedPointer<NetworkManager> network;
    QScopedPointer<NodesManager> nodesManager;
    QScopedPointer<PaintManager> paintManager;

    // Helper stuff (still independent init)
    QScopedPointer<WordFreqAnalyze> analyze;
    QScopedPointer<TagProcessor> tagProcessor;
    QScopedPointer<TextProcessor> textProcessor;

    // Dependent init
    QScopedPointer<GroupsManager> groupsManager;
    QScopedPointer<MainScene> scene;
    QScopedPointer<QQmlApplicationEngine> qmlEngine;
};

#endif  // MAINWINDOW_H
