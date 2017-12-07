#ifndef MYSCENEVIEW_H
#define MYSCENEVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QQuickImageProvider>
#include <QResizeEvent>
#include "glb.h"
//#include "mainscene.h"

class MainScene;

class MyView : public QGraphicsView, public QQuickImageProvider
{
    Q_OBJECT

public:
    MyView();

    QSize retImgSz = QSize(400,300);//Да, она публично доступна. Такие дела.

    void wheelEvent(QWheelEvent *evt);

    void mousePressEvent(QMouseEvent *evt)
    {
//        qDebug()<<"press"<<evt->buttons();
        newInfo( "press " + Glb::ptToStr( evt->pos() ) );
        newPos( evt->pos().x(),evt->pos().y() );
//        NodesList lst = Glb::scRef->getAllNodes();
//        for( TermNode *n : lst ) {
//            if(n->getMainRect().contains(evt->pos())) {
//                newInfo(n->getName());
//                break;
//            }
//        }
        QGraphicsView::mousePressEvent(evt);
    }

    void mouseReleaseEvent(QMouseEvent *evt)
    {
//        qDebug()<<"release"<<evt->buttons();
        newInfo( "release " + Glb::ptToStr( evt->pos() ) );
        QGraphicsView::mouseReleaseEvent(evt);
    }

    void mouseMoveEvent(QMouseEvent *evt)
    {
//        qDebug()<<"move"<<evt->buttons();
        newInfo( "move " + Glb::ptToStr( evt->pos() ) );
        QGraphicsView::mouseMoveEvent(evt);
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    bool event(QEvent *event){
//        qDebug()<<"eventMove"<<event->type();
        return QWidget::event(event); //Не знаю почему, но без этого не работает...
    }

public slots:

    void scaleUp();
    void scaleDown();

    void moveLeft() {
        moveView(QPointF(-10.0,0.0));
    }
    void moveRight() {
        moveView(QPointF(10.0,0.0));
    }
    void moveUp() {
        moveView(QPointF(0.0,-10.0));
    }
    void moveDown() {
        moveView(QPointF(0.0,10.0));
    }

    void setScSize(int width,int height) {
//        qDebug()<<"NEW SIZE"<<width<<height<<"prev"<<size()<<"screct"<<sceneRect();
//        resize(width,height);
//        adjustSize();
//        updateGeometry();
    }

    void setDragMove(bool drg) {
        if( drg )
            setDragMode(QGraphicsView::ScrollHandDrag);
        else
            setDragMode(QGraphicsView::NoDrag);
    }

signals:
    void newInfo(QString info);
    void newPos(int x,int y);

private:
    void moveView(QPointF move){
        QPointF pt = mapToScene(viewport()->rect().center());
        centerOn( pt + move );
    }

    int scaleCount = 0;
    QPointF centerPoint = QPointF();

};


#endif // MYSCENEVIEW_H
