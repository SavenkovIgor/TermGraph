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

enum SceneScale {
    up = 1,
    down = -1
};

class MyView : public QGraphicsView, public QQuickImageProvider
{
    Q_OBJECT

    QSize retImgSz = QSize(400,300);

public:
    MyView();

    void wheelEvent(QWheelEvent *evt);

    void mousePressEvent(QMouseEvent *evt)
    {
        newInfo( "press " + Glb::ptToStr( evt->pos() ) );
        newPos( evt->pos().x(),evt->pos().y() );
        QGraphicsView::mousePressEvent(evt);
    }

    void mouseReleaseEvent(QMouseEvent *evt)
    {
        newInfo( "release " + Glb::ptToStr( evt->pos() ) );
        QGraphicsView::mouseReleaseEvent(evt);
    }

    void mouseMoveEvent(QMouseEvent *evt)
    {
        newInfo( "move " + Glb::ptToStr( evt->pos() ) );
        QGraphicsView::mouseMoveEvent(evt);
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    bool event(QEvent *event){
        return QWidget::event(event); //Не знаю почему, но без этого не работает...
    }

public slots:

    void scaleUp();
    void scaleDown();

    void moveLeft()  { moveView(QPointF(-10.0, 0.0)); }
    void moveRight() { moveView(QPointF(10.0, 0.0));  }
    void moveUp()    { moveView(QPointF(0.0, -10.0)); }
    void moveDown()  { moveView(QPointF(0.0, 10.0));  }

    void setDragMove(bool drg) {
        if( drg )
            setDragMode(QGraphicsView::ScrollHandDrag);
        else
            setDragMode(QGraphicsView::NoDrag);
    }

    void setSceneSize(int width, int height)
    {
        retImgSz.setWidth(  width  );
        retImgSz.setHeight( height );
    }

signals:
    void newInfo(QString info);
    void newPos(int x,int y);

private:
    void scaleChange(SceneScale scaleChange);

    void moveView(QPointF move){
        QPointF pt = mapToScene(viewport()->rect().center());
        centerOn( pt + move );
    }

    int scaleCount = 0;
    QPointF centerPoint = QPointF();

};


#endif // MYSCENEVIEW_H
