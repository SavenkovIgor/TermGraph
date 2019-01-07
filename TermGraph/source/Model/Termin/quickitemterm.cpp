#include "quickitemterm.h"

QuickItemTerm::QuickItemTerm(QSqlRecord rec):
    PaintedTerm(rec),
    QQuickPaintedItem()
{

}

void QuickItemTerm::paint(QPainter *painter)
{
    // defaults
    QRectF rcBase = getNodeRect(CoordType::zeroPoint);
    QColor col = getBaseColor();
    int transpVal = 100;
//    setZValue(1);

    if (someoneHover || someoneSelect) {
        if (thisHovered /*|| isSelected()*/ || relativePaint) {
            if (/*isSelected() ||*/ thisHovered)
                col = getSelectedColor();
            col.setAlpha(255);
//            setZValue(2);
        } else {
            col.setAlpha(transpVal);
//            setZValue(1);
        }
    }

    QString str;
    str += getSmallName();

    //    str += ":" + QString::number( uid );
    //    str += " (" + QString::number( paintLevel ) + ")";
    //    str += " (" + QString::number( neighboursList.size() ) + ")";
    //    str += " (" + QString::number( edgesUpList.size() ) + ")";

    //    EdgeRefList ed;
    //    ed<<edgesUpList;
    //    ed<<edgesDownList;
    //    str += " :" + QString::number(ed.count());
    //    str += " :" + QString::number( personalAddInterval );
    //    str += " :" + Glb::ptToStr( scenePos() );
    //    QString testStrInner = " :" + QString::number( personalAddInterval );
    //    QString testStrInner = " :" + QString::number( newPosOffs );

    QBrush br(col, Qt::SolidPattern);

    QColor borderColor = AppStyle::Colors::Nodes::border;
    borderColor.setAlpha(col.alpha());
    painter->setPen(borderColor);
    painter->setBrush(br);

    qreal minSide = qMin(rcBase.width(), rcBase.height());
    int cornerRadius = qRound(minSide * 0.18);
    if (thisHovered)
        painter->drawRoundedRect(rcBase.adjusted(-1, -1, 1, 1), cornerRadius, cornerRadius);
    else
        painter->drawRoundedRect(rcBase, cornerRadius, cornerRadius);

    painter->setPen(AppStyle::Colors::Nodes::text);
    painter->drawText(rcBase.adjusted(1, 0, 1, 0), Qt::AlignCenter, str);

    if (atLearning()) {
        QRectF miniRc = QRectF(rcBase.topLeft(), QSizeF(11, 14));
        painter->setBrush(AppStyle::Colors::Nodes::atLearn);
        painter->drawRoundedRect(miniRc, 1, 1);
        painter->drawText(rcBase.topLeft() + QPointF(2, 12), QString::number(getRepNum()));
    }
}

QPointF QuickItemTerm::getPos() const  // TODO: Realize!
{
    return QPointF();
}

QPointF QuickItemTerm::getScenePos() const  // TODO: Realize!
{
    return QPointF();
}

void QuickItemTerm::movePosBy(qreal dx, qreal dy)  // TODO: Realize!
{

}
