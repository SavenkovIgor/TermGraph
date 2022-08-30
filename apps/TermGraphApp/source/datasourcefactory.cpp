// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "datasourcefactory.h"

#include <QHostAddress>

#include <CommonTools/Platform.h>
#include <TermDataConnection/DataStorageConnection.h>

#ifndef Q_OS_WASM
#include <TermDataStorage/LocalDataStorage.h>
#endif

#include "source/helpers/appsettings.h"

std::unique_ptr<DataStorageInterface> DataSourceFactory::defaultSource()
{
    if (Platform::isWasm())
        return server();

    if (Platform::isDesktop() || Platform::isMobile())
        return localDb();
}

std::unique_ptr<DataStorageInterface> DataSourceFactory::localDb()
{
#ifdef Q_OS_WASM
    return nullptr;
#else
    using namespace AppSettings;
    return std::make_unique<LocalDatabaseStorage>(Paths::defaultDatabaseFilePath(), Paths::backupFolder());
#endif
}

std::unique_ptr<DataStorageInterface> DataSourceFactory::server()
{
    auto address = QHostAddress("127.0.0.1");
    return std::make_unique<DataStorageConnection>(address);
}
