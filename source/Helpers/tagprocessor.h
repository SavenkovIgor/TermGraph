#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>

class TagProcessor : public QObject
{
    Q_OBJECT
public:
    explicit TagProcessor(QObject *parent = nullptr);

public slots:
    static QStringList extractTags(QString str, QString &errorString);
    QString addTagInPosition(int cursorPosition, QString str);
};

#endif  // TEXTPROCESSOR_H
