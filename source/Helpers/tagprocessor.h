#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>

class TagProcessor : public QObject
{
    Q_OBJECT
public:
    explicit TagProcessor(QObject *parent = nullptr);

    static QStringList extractTags(QString str, QString &errorString);
    static QString addTagInPosition(int cursorPosition, QString str);

signals:

public slots:
};

#endif // TEXTPROCESSOR_H
