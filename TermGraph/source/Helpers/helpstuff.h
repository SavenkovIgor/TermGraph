#ifndef HELPSTUFF_H
#define HELPSTUFF_H

#include <QString>
#include <QPoint>
#include <QPointF>

class HelpStuff {
public:
    static QString ptToStr(const int& x, const int& y, const QString& divider = "x");
    static QString ptToStr(const QPoint& pt, const QString& divider = "x");
    static QString ptToStr(const QPointF& pt, const QString& divider = "x");
};

#endif // HELPSTUFF_H
