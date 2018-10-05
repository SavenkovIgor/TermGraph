#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>

class TagProcessor : public QObject
{
    Q_OBJECT

    static const QChar leftBracket;
    static const QChar rightBracket;
    static const QString emptyBrackets;
public:
    enum class SearchDirection { left, right };

    explicit TagProcessor(QObject *parent = nullptr);

    static bool isBracket(QChar ch);

    static int searchWordBorder(const SearchDirection searchDirection, const QString& text, int cursorPosition);
    static QChar getNearesBracket(const SearchDirection searchDirection, const QString& text, int cursorPosition);
    static bool isInsideTag(const QString& text, int cursorPosition);

    static bool isPairedBrackets(QString text);
    static int getMaxDepthOfNestedBrackets(QString text);

    static QString getErrorFor(QString str);  // TODO: Realize!

    static int getLevDistance(const QString &src, const QString &dst);

    static bool isTagCorrespondToTermName(QString termName, QString tag);
public slots:
    static QStringList extractTags(QString str);
    QString addTagInPosition(int cursorPosition, QString str);
};

#endif  // TEXTPROCESSOR_H
