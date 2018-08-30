#ifndef HELPSTUFF_H
#define HELPSTUFF_H

#include <QString>
#include <QPoint>
#include <QPointF>

class HelpStuff{
public:
    static QString ptToStr(int x, int y, QString divider = "x");
    static QString ptToStr(QPoint pt, QString divider = "x");
    static QString ptToStr(QPointF pt, QString divider = "x");
};

#endif // HELPSTUFF_H
