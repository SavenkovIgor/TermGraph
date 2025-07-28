// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QStringView>

export module Link.LinksString;

import CommonTools.HandyTypes;
import Link.Link;
import Text.CharTools;
import Text.CheckingTextCursor;
import Text.TextCursor;
import Text.TextRange;

export class LinksString
{
public:
    LinksString(QStringView str)
        : mString(str)
        , mLinks(extractLinks(mString))
    {}

    LinksString(const LinksString& other) = default;

    QString           text() const { return mString.toString(); }
    const Link::List& links() const { return mLinks; }

    QString replaceLink(int index, QString text) const
    {
        auto sizeIndex      = Link::asListSize(index);
        auto [linkCut, pos] = mLinks[sizeIndex].cutted();
        linkCut.insert(pos, text);
        return linkCut;
    }

    // Static stuff
    static bool isValidLinksString(QStringView str)
    {
        int brCount = 0;

        for (auto ch : str) {
            if (ch == CharTools::leftBracket) {
                brCount++;
            }

            if (ch == CharTools::rightBracket) {
                brCount--;
            }

            if (brCount < 0 || brCount > 1) {
                return false;
            }
        }

        return brCount == 0;
    }

    /// Описание:
    /// Функция в пустой строке или строке с пробелами вернет {}
    /// Функция на границе строки обрамит крайнее слово
    /// При встрече уже обрамленного тега - ничего не сделает
    static QString addLink(QString str, int cursor)
    {
        // Проверка корректности курсора
        if (!TextCursor::isValidCursor(str, cursor)) {
            return str;
        }

        // Нельзя просто так упрощать строку - индекс курсора же не меняется
        // str = str.simplified();

        // Если мы уже находимся внутри тега - ничего не делаем
        if (Link::isCursorOnLink(str, cursor)) {
            return str;
        }

        auto word = TextRange::selectWord(str, cursor);

        //Сначала вставляем правую, потом левую из за смещения курсора
        str.insert(word.right().pos(), CharTools::rightBracket);
        str.insert(word.left().pos(), CharTools::leftBracket);

        return str;
    }

    static QString expandLinkRight(QString str, int cursor)
    {
        if (!TextCursor::isValidCursor(str, cursor)) {
            return str;
        }

        if (!Link::isCursorOnLink(str, cursor)) {
            return str;
        }

        // Move to right bracket
        auto rBracket = CheckingTextCursor::anyBracketOnRight(str, cursor, Direction::Right);

        assert(rBracket.right() == CharTools::rightBracket);

        auto lWord = CheckingTextCursor::leftWordBorder(str, rBracket.pos() + 1, Direction::Right);

        if (!lWord.check()) { // if not found
            return str;
        }

        auto lBracketMaybe = CheckingTextCursor::leftBracketOnRight(str, rBracket.pos() + 1, Direction::Right);

        // Protection from capturing next link
        if (lBracketMaybe.check()) {
            if (lWord.pos() > lBracketMaybe.pos()) {
                return str;
            }
        }

        auto rWord = CheckingTextCursor::rightWordBorder(str, lWord.pos(), Direction::Right);

        if (rBracket.check() && rWord.check()) {
            str.insert(rWord.pos(), CharTools::rightBracket);
            str.remove(rBracket.pos(), 1);
            return str;
        }

        return str;
    }

    static QString removeLink(QString str, int cursor)
    {
        // Проверка корректности курсора
        if (!TextCursor::isValidCursor(str, cursor)) {
            return str;
        }

        // Нельзя просто так упрощать строку - индекс курсора же не меняется
        // str = str.simplified();

        // Если мы не находимся внутри тега - ничего не делаем
        if (!Link::isCursorOnLink(str, cursor)) {
            return str;
        }

        auto leftBracket  = CheckingTextCursor::anyBracketOnLeft(str, cursor, Direction::Left);
        auto rightBracket = CheckingTextCursor::anyBracketOnRight(str, cursor, Direction::Right);

        assert(leftBracket.check());
        assert(rightBracket.check());

        //Сначала удаляем правую, потом левую из за смещения индексов
        str.remove(rightBracket.pos(), 1);
        str.remove(leftBracket.pos() - 1, 1);

        return str;
    }

private: // Methods
    static int getCount(QStringView strView)
    {
        if (!isValidLinksString(strView)) {
            return 0;
        }

        auto str = strView.toString();
        return asInt(str.count(CharTools::leftBracket));
    }

    static Link::List extractLinks(QStringView strView)
    {
        Link::List ret;

        if (!isValidLinksString(strView)) {
            return ret;
        }

        int count = getCount(strView);

        for (int i = 0; i < count; i++) {
            int strPos = -1;

            for (int j = 0; j <= i; j++) {
                strPos = asInt(strView.indexOf(CharTools::leftBracket, strPos + 1));
            }

            auto link = Link::select(strView, strPos + 1);

            assert(link.has_value());

            ret.push_back(link.value());
        }

        return ret;
    }

private: // Members
    const QStringView mString;
    const Link::List  mLinks;
};
