#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>

class TextProcessor : public QObject
{
    Q_OBJECT

public:
    TextProcessor(QObject* parent = nullptr);

public slots:
    bool isTermWithDefinition(const QString& def) const;

    QString getTerm(const QString& def) const;
    QString getDefinition(const QString& def) const;

private:
    QStringList splitters;

    int splitterIndex(const QString& str) const;
};

#endif // TEXTPROCESSOR_H
