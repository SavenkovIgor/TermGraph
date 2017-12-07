#include "glb.h"

Qt::Orientation Glb::ori = Qt::Horizontal;

Glb::Glb()
{

}

bool Glb::isVertical(){
    return ori == Qt::Vertical;
}

bool Glb::isHorizontal(){
    return ori == Qt::Horizontal;
}

QObject *Glb::getObj(QQuickWidget *wg, QString objName)
{
    QObject* obj = wg->rootObject()->findChild<QObject*>(objName);
    if( obj == nullptr )
        qDebug()<<"Object"<<objName<<"not found in"<<wg->objectName()<<"!";
    return obj;
}

QVariant Glb::getProp(QQuickWidget *wg, QString objName, QString prop)
{
    QObject* obj = getObj(wg,objName);
    if( obj == nullptr )
        return QVariant();
    return obj->property(prop.toStdString().c_str());
}

void Glb::setProp(QQuickWidget *wg, QString objName, QString prop, QVariant val)
{
    QObject* obj = getObj(wg,objName);
    if( obj == nullptr )
        return;
    obj->setProperty(prop.toStdString().c_str(),val);
}

int Glb::getLevDistance(const QString &src, const QString &dst)
{
  const int m = src.size();
  const int n = dst.size();
  if (m == 0) {
    return n;
  }
  if (n == 0) {
    return m;
  }

  std::vector< std::vector<int> > matrix(m + 1);

  for (int i = 0; i <= m; ++i) {
    matrix[i].resize(n + 1);
    matrix[i][0] = i;
  }
  for (int i = 0; i <= n; ++i) {
    matrix[0][i] = i;
  }

  int above_cell, left_cell, diagonal_cell, cost;

  for(int i = 1; i <= m; ++i) {
    for(int j = 1; j <= n; ++j) {
      cost = src[i - 1] == dst[j - 1] ? 0 : 1;
      above_cell = matrix[i - 1][j];
      left_cell = matrix[i][j - 1];
      diagonal_cell = matrix[i - 1][j - 1];
      matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
    }
  }

  return matrix[m][n];
}

QString Glb::ptToStr(int x, int y, QString divider) {
    return QString::number(x) + divider + QString::number(y);
}

QString Glb::ptToStr(QPoint pt, QString divider){
    return ptToStr( pt.x(), pt.y(), divider );
}

QString Glb::ptToStr(QPointF pt, QString divider){
    return ptToStr( pt.toPoint(), divider );
}
