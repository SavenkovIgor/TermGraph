#ifndef CLOUDSERVICES_H
#define CLOUDSERVICES_H

#include <QObject>

#include "dropboxconnection.h"

class CloudServicesWrapper : public QObject
{
    Q_OBJECT
public:
    explicit CloudServicesWrapper(QObject *parent = nullptr);

signals:

public slots:
private:
    DropboxConnection *dropConn;
};

#endif  // CLOUDSERVICES_H
