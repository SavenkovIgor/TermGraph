// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "datasourcefactory.h"

#include <QHostAddress>

#include <CommonTools/Platform.h>
#include <CommonTools/NetworkTools.h>
#include <StaticDataStorage/StaticDataStorage.h>
#include <TermDataConnection/DataStorageConnection.h>

#ifndef Q_OS_WASM
#include <TermDataStorage/LocalDataStorage.h>
#include "source/helpers/appsettings.h"
#endif

std::unique_ptr<DataStorageInterface> DataSourceFactory::defaultSource()
{
    if constexpr (Platform::isWasm()) {
        if constexpr (Platform::isStaticDataStorage()) {
            return std::make_unique<StaticDataStorage>();
        } else {
            return server();
        }
    }

    if constexpr (Platform::isDesktop() || Platform::isMobile()) {
        return localDb();
    }

    Q_UNREACHABLE();
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
//    auto address = QUrl("http://127.0.0.1");
//    address.setPort(NetworkTools::defaultPort);
    auto address = QUrl("https://termgraph.app");
    return std::make_unique<DataStorageConnection>(address);
}
