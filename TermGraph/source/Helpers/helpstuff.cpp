#include "helpstuff.h"

QString HelpStuff::ptToStr(const int& x, const int& y) {
    return "point x:" + QString::number(x) + " y:" + QString::number(y);
}

QString HelpStuff::ptToStr(const QPoint& pt) {
    return ptToStr(pt.x(), pt.y());
}

QString HelpStuff::ptToStr(const QPointF& pt) {
    return ptToStr(pt.toPoint());
}

QString HelpStuff::sizeToStr(const int &width, const int &height)
{
    return "size w:" + QString::number(width) + " h:" + QString::number(height);
}

QString HelpStuff::sizeToStr(const QSize &size)
{
    return sizeToStr(size.width(), size.height());
}

QString HelpStuff::sizeToStr(const QSizeF &size)
{
    return sizeToStr(size.toSize());
}

QString HelpStuff::rectToStr(const QRect &rect)
{
    return ptToStr(rect.topLeft()) + " " + sizeToStr(rect.size());
}

QString HelpStuff::rectToStr(const QRectF &rect)
{
    return rectToStr(rect.toRect());
}
