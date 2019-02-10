#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>

class TextProcessor : public QObject
{
    Q_OBJECT

public:
    TextProcessor(QObject* parent = nullptr);

public slots:
    bool IsTermWithDefinition(const QString& def) const;

    QString getTerm(const QString& def) const;
    QString getDefinition(const QString& def) const;

private:
    static QStringList splitters;
};

#endif // TEXTPROCESSOR_H
