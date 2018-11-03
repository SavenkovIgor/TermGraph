#include "helpstuff.h"

QString HelpStuff::ptToStr(const int& x, const int& y, const QString& divider) {
    return QString::number(x) + divider + QString::number(y);
}

QString HelpStuff::ptToStr(const QPoint& pt, const QString& divider) {
    return ptToStr(pt.x(), pt.y(), divider);
}

QString HelpStuff::ptToStr(const QPointF& pt, const QString& divider) {
    return ptToStr(pt.toPoint(), divider);
}
