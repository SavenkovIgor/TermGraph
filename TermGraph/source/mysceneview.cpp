#include "mysceneview.h"

MyView::MyView() :
    QQuickImageProvider( QQuickImageProvider::Image )
{
    setMouseTracking( true );
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);
}


void MyView::wheelEvent(QWheelEvent *evt)
{
    if( evt->modifiers() & Qt::ControlModifier ) {
        if(evt->angleDelta().y() > 0) {
            scaleUp();
        } else if(evt->angleDelta().y() < 0) {
            scaleDown();
        }

        evt->accept();
    }
    QGraphicsView::wheelEvent(evt);
}

QImage MyView::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage img = QImage(retImgSz,QImage::Format_ARGB32);
    QPainter pt(&img);

    if( pt.isActive() ) {
        pt.setRenderHint(QPainter::Antialiasing);
        pt.fillRect(img.rect(), Colors::Scene::background);

        this->setMinimumSize(img.size());
        this->setBaseSize(img.size());

        this->render(&pt,img.rect(),img.rect());
        return img;
    }
    return QImage();
}

void MyView::scaleChange(SceneScale scaleChange)
{
    const int maxScaleCount = 10;
    const int minScaleCount = -10;

    scaleCount += scaleChange;

    if( minScaleCount < scaleCount && scaleCount < maxScaleCount) {
        switch (scaleChange) {
        case SceneScale::up:
            scale(1.1,1.1);
            break;
        case SceneScale::down:
            scale(0.9,0.9);
            break;
        }
    }

    scaleCount = qBound(minScaleCount,scaleCount,maxScaleCount);
}
