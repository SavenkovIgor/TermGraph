#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <functional>

#include <QObject>

class TagProcessor : public QObject
{
    Q_OBJECT

    constexpr static char leftBracket = '{';
    constexpr static char rightBracket = '}';
    constexpr static auto emptyBrackets = "{}";

public:
    enum class SearchDirection { left, right };

    explicit TagProcessor(QObject *parent = nullptr);

    static inline bool isBracket(const QChar &ch);

    static int searchWordBorder(const SearchDirection direction, const QString& text, int cursorPos);
    static QChar getNearesBracket(const SearchDirection direction, const QString& text, int cursorPos);
    static bool isInsideTag(const QString& text, int cursorPos);

    static bool isPairedBrackets(QString text);
    static int getMaxDepthOfNestedBrackets(QString text);

    static QString getErrorFor(QString str);  // TODO: Realize!

    static int getLevDistance(const QString &src, const QString &dst);

    static bool isTagCorrespondToTermName(QString termName, QString tag);

    static int getCursorPosition(
            const SearchDirection& direction,
            const QString& text,
            int cursorPos,
            std::function<bool(QChar)> exitCondition);

public slots:
    static QStringList extractTags(QString str);
    QString addTagInPosition(int cursorPosition, QString str);
    QString extendRight(int cursorPosition, QString str);
};

#endif  // TEXTPROCESSOR_H
