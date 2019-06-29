#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

#include "source/Model/reminder.h"
#include "source/mainscene.h"
#include "source/paintqueuemanager.h"
#include "source/databaseWorks/database.h"
#include "source/Model/wordfreqanalyze.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Helpers/textprocessor.h"
#include "source/Helpers/appconfig.h"
#include "source/Managers/syncmanager.h"

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow() = default;

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
    QScopedPointer<SyncManager> syncManager;
    QScopedPointer<MainScene> scene;
    QScopedPointer<QQmlApplicationEngine> qmlEngine;
};

#endif  // MAINWINDOW_H
