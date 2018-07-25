#ifndef EDGE_H
#define EDGE_H

#include "glb.h"

class TermNode;

enum EdgeType{
    standart,
    termin,
    description
};

class Edge : public QGraphicsItem
{
//    Q_OBJECT

private:
    TermNode* toRoot;
    TermNode* toLeaf;

    EdgeType type = termin;

    QColor getEdgeColor();

public:
    bool wide = false;

public:
    Edge( TermNode *toRoot, TermNode *toLeaf, EdgeType type = EdgeType::termin );

    TermNode* getRoot() { return toRoot; }
    TermNode* getLeaf() { return toLeaf; }

    bool hasNode(TermNode *nd);

    bool isSameEdge(TermNode* rt,TermNode* br);
    bool isInGroupEdge();
    TermNode* getOtherSide(TermNode* n);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*);

    int getLayerDistance();

    qreal getXProjection();
    qreal getYProjection();

//    QPointF swP1 = QPointF();
    QPointF swPtRoot = QPointF();
    QPointF swPtBran = QPointF();
    QLineF getLine( bool swap = false);
};

#endif // EDGE_H
