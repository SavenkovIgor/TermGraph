#ifndef CHECKINGTEXTCURSOR_H
#define CHECKINGTEXTCURSOR_H

#include <QStringView>

#include "source/Helpers/text/chartools.h"
#include "source/Helpers/text/textcursor.h"

class CheckingTextCursor : public TextCursor
{
public:
    using Condition = CharTools::ShortCondition;

    CheckingTextCursor(QStringView strView, int pos, Condition leftChecker, Condition rightChecker);

    bool check() const;

    bool search(Direction dir);

    static CheckingTextCursor rightWordBorder(QStringView strView, int pos = 0);
    static CheckingTextCursor leftWordBorder(QStringView strView, int pos = 0);

    static CheckingTextCursor leftBracketOnRight(QStringView strView, int pos = 0);
    static CheckingTextCursor rightBracketOnLeft(QStringView strView, int pos = 0);
    static CheckingTextCursor anyBracketOnRight(QStringView strView, int pos = 0);
    static CheckingTextCursor anyBracketOnLeft(QStringView strView, int pos = 0);

private:
    const Condition mLeftChecker;
    const Condition mRightChecker;
};

#endif // CHECKINGTEXTCURSOR_H
