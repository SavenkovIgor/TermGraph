/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QCoreApplication>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>

#include "source/Helpers/tagprocessor.h"
#include "source/Managers/linkshardeningmanager.h"
#include "source/Managers/notificationmanager.h"
#include "source/Model/Termin/termdatawrapper.h"
#include "source/Model/TerminGroup/termgroup.h"

void registerUserQmlTypes()
{
    qmlRegisterSingletonInstance("Api", 1, 0, "Notification", &NotificationManager::instance());
    qmlRegisterSingletonInstance("Api", 1, 0, "TagProcessor", &TagProcessor::instance());

    qmlRegisterUncreatableType<TermDataWrapper>("Api",
                                                1,
                                                0,
                                                "TermDataWrapper",
                                                "Please use TermDataWrapper from backend");

    qmlRegisterType<LinksHardeningManager>("Api", 1, 0, "LinksHardeningManager");

    qmlRegisterAnonymousType<TermGroup>("Api", 1);
}

Q_COREAPP_STARTUP_FUNCTION(registerUserQmlTypes)
