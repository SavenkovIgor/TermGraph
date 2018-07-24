#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>

class TagProcessor : public QObject
{
    Q_OBJECT
    static const QChar leftBracket;
    static const QChar rightBracket;
public:
    explicit TagProcessor(QObject *parent = nullptr);

    static bool isPairedBrackets(QString text);
    static int getMaxDepthOfNestedBrackets(QString text);

public slots:
    static QStringList extractTags(QString str, QString &errorString);
    QString addTagInPosition(int cursorPosition, QString str);
};

#endif  // TEXTPROCESSOR_H
