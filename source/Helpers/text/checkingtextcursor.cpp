#include "source/Helpers/text/checkingtextcursor.h"

CheckingTextCursor::CheckingTextCursor(QStringView                   strView,
                                       int                           pos,
                                       CheckingTextCursor::Condition leftChecker,
                                       CheckingTextCursor::Condition rightChecker)
    : TextCursor(strView, pos)
    , mLeftChecker(leftChecker)
    , mRightChecker(rightChecker)
{}

bool CheckingTextCursor::check() const
{
    return mLeftChecker(left().value_or(QChar())) && mRightChecker(right().value_or(QChar()));
}

bool CheckingTextCursor::search(Direction dir)
{
    do {
        if (check())
            return true;

        move(dir);
    } while (canMove(dir));

    return check();
}

CheckingTextCursor CheckingTextCursor::rightWordBorder(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::isLetterOrNumber, CharTools::notLetterOrNumber);
}

CheckingTextCursor CheckingTextCursor::leftWordBorder(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::notLetterOrNumber, CharTools::isLetterOrNumber);
}

CheckingTextCursor CheckingTextCursor::leftBracketOnRight(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::any, CharTools::isLeftBracket);
}

CheckingTextCursor CheckingTextCursor::rightBracketOnLeft(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::isRightBracket, CharTools::any);
}

CheckingTextCursor CheckingTextCursor::anyBracketOnRight(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::any, CharTools::isBracket);
}

CheckingTextCursor CheckingTextCursor::anyBracketOnLeft(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::isBracket, CharTools::any);
}
