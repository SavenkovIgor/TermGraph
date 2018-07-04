#ifndef TERMGRAPH_H
#define TERMGRAPH_H

#include <QObject>

class TermGraph : public QObject
{
    Q_OBJECT
public:
    explicit TermGraph(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TERMGRAPH_H