#pragma once

#include <QObject>

//#include "qtdropbox.h"

class DropboxConnection : public QObject
{
    Q_OBJECT
public:
    explicit DropboxConnection(QObject *parent = nullptr);

private:
//    QDropbox *dropbox;
};
