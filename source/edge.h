#ifndef EDGE_H
#define EDGE_H

#include "glb.h"

class TermNode;

class Edge : public QGraphicsItem
{
//    Q_OBJECT

private:
    TermNode *toRoot;
    TermNode *toBrnch;

public:
    bool wide = false;

public:
    Edge( TermNode *toRoot, TermNode *toBrnch );

    TermNode *getRoot() { return toRoot;  }
    TermNode *getBrnch(){ return toBrnch; }

    bool hasNode(TermNode *nd);

    bool isSameEdge(TermNode *rt,TermNode *br);
    bool isDiffGroupEdge();
    TermNode *getOtherSide(TermNode *n);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    int getLayerDistance();

    qreal getXProjection();
    qreal getYProjection();

//    QPointF swP1 = QPointF();
    QPointF swPtRoot = QPointF();
    QPointF swPtBran = QPointF();
    QLineF getLine( bool swap = false);
};

#endif // EDGE_H
