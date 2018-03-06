#include "cloudservices.h"

CloudServicesWrapper::CloudServicesWrapper(QObject *parent) : QObject(parent)
{
    dropConn = new DropboxConnection();
}
