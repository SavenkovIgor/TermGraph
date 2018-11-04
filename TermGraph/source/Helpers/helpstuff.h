#ifndef HELPSTUFF_H
#define HELPSTUFF_H

#include <QString>
#include <QPointF>
#include <QSizeF>
#include <QRectF>

class HelpStuff {
public:
    static QString ptToStr(const int& x, const int& y);
    static QString ptToStr(const QPoint& pt);
    static QString ptToStr(const QPointF& pt);

    static QString sizeToStr(const int& width, const int& height);
    static QString sizeToStr(const QSize& size);
    static QString sizeToStr(const QSizeF& size);

    static QString rectToStr(const QRect& rect);
    static QString rectToStr(const QRectF& rect);
};

#endif // HELPSTUFF_H
