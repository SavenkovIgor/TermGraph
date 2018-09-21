#include "termnode.h"

TermNode::TermNode(QSqlRecord rec):
    PaintedNode(rec),
    QGraphicsItem()
{
    adjustSizeForName();

    //    setFlag( QGraphicsItem::ItemIsSelectable,true );
    //    setFlag(QGraphicsItem::ItemIsMovable,false);

    setAcceptHoverEvents(true);
    setZValue(1);
}

QRectF TermNode::boundingRect() const
{
    return getInnerNodeRect();
}

void TermNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // defaults
    QRectF rcBase = getInnerNodeRect();
    QColor col = getBaseColor();
    int transpVal = 100;
    setZValue(1);

    if (someoneHover || someoneSelect) {
        if (thisHovered || isSelected() || relativePaint) {
            if (isSelected() || thisHovered)
                col = getSelectedColor();
            col.setAlpha(255);
            setZValue(2);
        } else {
            col.setAlpha(transpVal);
            setZValue(1);
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

void TermNode::hoverEnterEvent(QGraphicsSceneHoverEvent *evt)
{
    someoneHover = true;
    thisHovered = true;

    relativePaint = true;

    setRelatedPaint(true);

    QGraphicsItem::hoverEnterEvent(evt);
}

void TermNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *evt)
{
    thisHovered = false;
    someoneHover = false;

    relativePaint = false;

    if (!isSelected())
        setRelatedPaint(false);

    QGraphicsItem::hoverLeaveEvent(evt);
}

void TermNode::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsItem::mousePressEvent(evt);
}

QString TermNode::getDebugString() {
    QStringList p;
    QString tmp;

    p << HelpStuff::ptToStr(pos());

    tmp = " childs:";
    QList<QGraphicsItem*> childs = childItems();
    for (int i = 0; i < childs.size(); i++) {
        TermNode* n = dynamic_cast<TermNode*>(childs[i]);
        tmp += " " + n->getUuid().toString();
    }
    p << HelpStuff::ptToStr(scenePos());

    QString addInterf;
    addInterf += QString::number(getIntersections()) + " ";
    addInterf += QString::number(getIntersections(true));
    p << addInterf;

    p << " upLevels:" + QString::number(getUpLevels() - getPaintLevel());
    //        addInterf += QString::number(personalAddInterval) + " ";
    p << "Uuid" << getUuid().toString();
    return p.join("\n");
}

QPointF TermNode::getPos() const
{
    return this->pos();
}

QPointF TermNode::getScenePos() const
{
    return this->scenePos();
}

void TermNode::movePosBy(qreal dx, qreal dy)
{
    moveBy(dx,dy);
}

void TermNode::PrepareGeometryChangeCall()
{
    prepareGeometryChange();
}
