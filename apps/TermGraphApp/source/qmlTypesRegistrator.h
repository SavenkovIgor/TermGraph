// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QCoreApplication>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>

#include "source/helpers/tagprocessor.h"
#include "source/managers/linkshardeningmanager.h"
#include "source/managers/notificationmanager.h"
#include "source/model/enums.h"
#include "source/model/group/termgroup.h"
#include "source/model/term/termdatawrapper.h"

void registerUserQmlTypes()
{
    qmlRegisterSingletonInstance("Api", 1, 0, "Notification", &NotificationManager::instance());
    qmlRegisterSingletonInstance("Api", 1, 0, "TagProcessor", &TagProcessor::instance());

    qmlRegisterUncreatableType<TermDataWrapper>("Api",
                                                1,
                                                0,
                                                "termDataWrapper",
                                                "Please use TermDataWrapper from backend");

    qmlRegisterUncreatableType<NodeType>("Api", 1, 0, "NodeType", "Just enum");
    qmlRegisterUncreatableType<EdgeType>("Api", 1, 0, "EdgeType", "Just enum");
    qmlRegisterUncreatableType<EdgeSelection>("Api", 1, 0, "EdgeSelection", "Just enum");

    qmlRegisterType<LinksHardeningManager>("Api", 1, 0, "LinksHardeningManager");

    qmlRegisterAnonymousType<TermGroup>("Api", 1);
}

Q_COREAPP_STARTUP_FUNCTION(registerUserQmlTypes)
