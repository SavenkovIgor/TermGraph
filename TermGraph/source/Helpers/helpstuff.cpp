#include "helpstuff.h"

QString HelpStuff::ptToStr(int x, int y, QString divider) {
    return QString::number(x) + divider + QString::number(y);
}

QString HelpStuff::ptToStr(QPoint pt, QString divider){
    return ptToStr(pt.x(), pt.y(), divider);
}

QString HelpStuff::ptToStr(QPointF pt, QString divider){
    return ptToStr(pt.toPoint(), divider);
}
