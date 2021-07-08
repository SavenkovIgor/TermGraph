/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/helpers/link/linksstring.h"

#include "source/helpers/text/chartools.h"
#include "source/helpers/text/checkingtextcursor.h"

LinksString::LinksString(QStringView str)
    : mString(str)
    , mLinks(extractLinks(mString))
{}

QString LinksString::text() const { return mString.toString(); }

const Link::List &LinksString::links() const { return mLinks; }

QString LinksString::replaceLink(int index, QString text) const
{
    auto [linkCut, pos] = mLinks[index].cutted();
    linkCut.insert(pos, text);
    return linkCut;
}

bool LinksString::isValidLinksString(QStringView str)
{
    int brCount = 0;

    for (auto ch : str) {
        if (ch == CharTools::leftBracket)
            brCount++;

        if (ch == CharTools::rightBracket)
            brCount--;

        if (brCount < 0 || brCount > 1)
            return false;
    }

    return brCount == 0;
}

/// Описание:
/// Функция в пустой строке или строке с пробелами вернет {}
/// Функция на границе строки обрамит крайнее слово
/// При встрече уже обрамленного тега - ничего не сделает
QString LinksString::addLink(QString str, int cursor)
{
    // Проверка корректности курсора
    if (!TextCursor::isValidCursor(str, cursor))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы уже находимся внутри тега - ничего не делаем
    if (Link::isCursorOnLink(str, cursor))
        return str;

    auto word = TextRange::selectWord(str, cursor);

    //Сначала вставляем правую, потом левую из за смещения курсора
    str.insert(word.right().pos(), CharTools::rightBracket);
    str.insert(word.left().pos(), CharTools::leftBracket);

    return str;
}

QString LinksString::expandLinkRight(QString str, int cursor)
{
    if (!TextCursor::isValidCursor(str, cursor))
        return str;

    if (!Link::isCursorOnLink(str, cursor))
        return str;

    // Move to right bracket
    auto rBracket = CheckingTextCursor::anyBracketOnRight(str, cursor, Direction::Right);

    assert(rBracket.right() == CharTools::rightBracket);

    auto lWord = CheckingTextCursor::leftWordBorder(str, rBracket.pos() + 1, Direction::Right);

    if (!lWord.check()) // if not found
        return str;

    auto lBracketMaybe = CheckingTextCursor::leftBracketOnRight(str, rBracket.pos() + 1, Direction::Right);

    // Protection from capturing next tag
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

QString LinksString::removeLink(QString str, int cursor)
{
    // Проверка корректности курсора
    if (!TextCursor::isValidCursor(str, cursor))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы не находимся внутри тега - ничего не делаем
    if (!Link::isCursorOnLink(str, cursor))
        return str;

    auto leftBracket  = CheckingTextCursor::anyBracketOnLeft(str, cursor, Direction::Left);
    auto rightBracket = CheckingTextCursor::anyBracketOnRight(str, cursor, Direction::Right);

    assert(leftBracket.check());
    assert(rightBracket.check());

    //Сначала удаляем правую, потом левую из за смещения индексов
    str.remove(rightBracket.pos(), 1);
    str.remove(leftBracket.pos() - 1, 1);

    return str;
}

int LinksString::getCount(QStringView strView)
{
    if (!isValidLinksString(strView))
        return 0;

    auto str = strView.toString();
    return str.count(CharTools::leftBracket);
}

Link::List LinksString::extractLinks(QStringView strView)
{
    Link::List ret;

    if (!isValidLinksString(strView))
        return ret;

    int count = getCount(strView);

    for (int i = 0; i < count; i++) {
        int strPos = -1;

        for (int j = 0; j <= i; j++)
            strPos = strView.indexOf(CharTools::leftBracket, strPos + 1);

        auto link = Link::select(strView, strPos + 1);

        assert(link.has_value());

        ret.push_back(link.value());
    }

    return ret;
}
