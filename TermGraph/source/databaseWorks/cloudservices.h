#pragma once

#include <QObject>

#include "dropboxconnection.h"

class CloudServicesWrapper : public QObject
{
    Q_OBJECT
public:
    explicit CloudServicesWrapper(QObject *parent = nullptr);

private:
    DropboxConnection *dropConn;
};
