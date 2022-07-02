// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/TermDataConnection/NetworkThread.h"

NetworkThread::NetworkThread()
    : QThread()
{
    start();
}

NetworkThread::~NetworkThread() { exit(); }

void NetworkThread::run()
{
    manager.reset(new QNetworkAccessManager());
    exec();
}
