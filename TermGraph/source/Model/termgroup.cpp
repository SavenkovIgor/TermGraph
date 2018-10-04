#include "termgroup.h"

int TermGroup::animSpeed = 300;

TermGroup::TermGroup(QSqlRecord rec, QObject *parent):
    QObject(parent)
{
    QString groupName = rec.value(TermGroupColumn::name).toString();
    this->grNmItem = new TGroupName(groupName);

    this->groupUuid = QUuid(rec.value(TermGroupColumn::longUID).toString());
    this->type = static_cast<GroupType>(rec.value(TermGroupColumn::type).toInt());
}

TermGroup::TermGroup(QJsonDocument doc, QObject *parent):
    QObject(parent)
{
    QJsonObject jsonObject = doc.object();

    this->groupUuid = QUuid(jsonObject.value("longUID").toString());
    this->type = static_cast<GroupType>(jsonObject.value("type").toInt());

    QString groupName = jsonObject.value("name").toString();
    this->grNmItem = new TGroupName( groupName );
}

void TermGroup::initNewNodes()
{
    loadEdges();

    baseRect = new QGraphicsRectItem(QRectF(QPoint(0, 0), QSizeF(10.0, 10.0)), nullptr);
    baseRect->setZValue(1);

    treeRect = new QGraphicsRectItem(nullptr);
    treeRect->setParentItem(baseRect);
    treeRect->setBrush(AppStyle::Colors::testColor);

    orphansRect = new QGraphicsRectItem(nullptr);
    orphansRect->setParentItem(baseRect);

    // Скрываем рамку
    hideRect(treeRect);
    hideRect(orphansRect);

    //    centerRect  = new QGraphicsRectItem( nullptr );

    switch (getType()) {
    case freeEdges:
        baseRect->setBrush(AppStyle::Colors::Groups::backgroundFreeConnections);
        break;
    case terms:
        baseRect->setBrush(AppStyle::Colors::Groups::backgroundTerms);
        break;
    }
    baseRect->setPen(AppStyle::Colors::Groups::border);

    this->grNmItem->setParentItem(baseRect);

    addNodesToParents();
    addEdgesToParents();

    setLayers();
    setTreeCoords();
    setNeighbours();
    setOrphCoords();

    checkSwapTimer.setSingleShot(false);
    setAnimSpeed(300);
    connect(&checkSwapTimer, SIGNAL(timeout()), SLOT(checkSwap()));

    animTimer.setSingleShot(false);
    animTimer.setInterval(50);
    connect(&animTimer, SIGNAL(timeout()), SLOT(animateGroup()));

    animGrp.addAnimation(&swAnim1);
    animGrp.addAnimation(&swAnim2);

    swAnim1.setPropertyName("pos");
    swAnim2.setPropertyName("pos");
    swAnim1.setEasingCurve(QEasingCurve::InOutQuad);
    swAnim2.setEasingCurve(QEasingCurve::InOutQuad);
}

TermGroup::~TermGroup()
{
    checkSwapTimer.stop();
    animTimer.stop();

    delete grNmItem;
    delete treeRect;
    delete orphansRect;
    delete baseRect;
}

bool TermGroup::checkJson(QJsonDocument doc)  // TODO: Вынести это отсюда!
{
    QJsonObject json = doc.object();

    QStringList checkKeys;
    checkKeys << "longUID";
    checkKeys << "name";
    checkKeys << "type";
    checkKeys << "nodesList";

    for (auto key : checkKeys) {
        if (!json.contains(key)) {
            return false;
        }
    }

    return true;
}

GroupType TermGroup::getType()
{
    return type;
}

QString TermGroup::getTypeString()
{
    return getTypesMap()[getType()];
}

QStringList TermGroup::getTypesNames()
{
    QMap<GroupType, QString> map = getTypesMap();
    QStringList ret;
    for (QString s : map) {
        ret << s;
    }

    return ret;
}

QMap<GroupType, QString> TermGroup::getTypesMap()
{
    QMap<GroupType, QString> ret;
    ret[freeEdges] = "Свободные связи";
    ret[terms]     = "Термины";
    return ret;
}

void TermGroup::loadNodes(GraphicItemTerm::List newNodes)
{
    nodesList.clear();
    for (GraphicItemTerm* node : newNodes) {
        if (node->getGroupUuid() != groupUuid) {
            qDebug() << "NodeLoad error for node:" << node->getUuid().toString();
            continue;
        }
        nodesList << node;
    }
    initNewNodes();
}

void TermGroup::loadEdges()
{
    if (getType() == GroupType::terms) {
        edgesList << searchConnections();
    }
}

void TermGroup::addNodesToParents()
{
    for (GraphicItemTerm* node : getInTreeNodes()) {
        node->setSceneParent(treeRect);
    }

    for (GraphicItemTerm* node : getOrphanNodes()) {
        node->setSceneParent(orphansRect);
    }
}

void TermGroup::addEdgesToParents()
{
    for (Edge* edge : getAllEdges()) {
        edge->setSceneParent(treeRect);
    }
}

QSizeF TermGroup::getVerticalStackedSize(GraphicItemTerm::List lst) const
{
    qreal width = 0.0;
    qreal height = 0.0;

    for (GraphicItemTerm* node : lst) {
        QSizeF sz = node->getFrameRect(CoordType::zeroPoint).size();
        height += sz.height();
        width = qMax(width, sz.width());
    }
    return QSizeF(width, height);
}

void TermGroup::swapNodes(GraphicItemTerm* n1, GraphicItemTerm* n2)
{
    if (animGrp.state() != QAbstractAnimation::Stopped)
        return;  // Very important instruction.
    // Because of lags, timer can still work, but animation will slow down and it cause
    // animation lags

    currAnimLevel = n1->getPaintLevel();

    QRectF rc1 = n1->getNodeRect(CoordType::local);
    QRectF rc2 = n2->getNodeRect(CoordType::local);

    QPointF pos1, pos2;

    if (rc1.top() < rc2.top()) {
        pos1 = QPointF(n1->pos().x() + rc1.width()/2 - rc2.width()/2, n1->pos().y());
        pos2 = QPointF(n2->pos().x() + rc2.width()/2 - rc1.width()/2, n2->pos().y() - rc1.height() + rc2.height());
    } else {
        pos1 = QPointF(n1->pos().x() + rc1.width()/2-rc2.width()/2, n1->pos().y() - rc2.height() + rc1.height());
        pos2 = QPointF(n2->pos().x() + rc2.width()/2-rc1.width()/2, n2->pos().y());
    }

    swAnim1.setTargetObject(n1);
    swAnim1.setStartValue(n1->pos());
    swAnim1.setEndValue(pos2);

    swAnim2.setTargetObject(n2);
    swAnim2.setStartValue(n2->pos());
    swAnim2.setEndValue(pos1);

    animGrp.start();
}

QJsonDocument TermGroup::getJsonDoc()
{
    QJsonDocument doc;
//    obj.insert("uid",     QJsonValue(grUid));
    QJsonObject obj;
    obj.insert(TermGroupColumn::longUID, QJsonValue(this->groupUuid.toString()));
    obj.insert(TermGroupColumn::name, QJsonValue(getName()));
    obj.insert(TermGroupColumn::type, QJsonValue(this->getType()));

    QJsonArray ndArr;

    for (GraphicItemTerm* node : nodesList) {
        ndArr.append(node->toJson());
    }
    obj.insert("nodesList", ndArr);

    doc.setObject(obj);
    return doc;
}

void TermGroup::startAnimation()
{
    animTimer.start();
    checkSwapTimer.start();
    grNmItem->timerStarted();
}

void TermGroup::stopAnimation()
{
    animTimer.stop();
    checkSwapTimer.stop();
    grNmItem->timerStopped();
}

void TermGroup::sceneUpdateSignal()
{
    updateGroupFrame();
}

void TermGroup::checkSwap()
{
    QTime t;
    t.start();
    int maxLevel = getLayersCount();

    for (int i = 1; i <= maxLevel; i++) {  // TODO: Обдумать этот момент i=1 because we need to ignore roots.
        GraphicItemTerm::List levLst = getNodesInLevel(i);

        for (int j = 0; j < levLst.size() - 1; j++) {
            int inter = levLst[j]->getIntersections();

            QPointF pt1 = levLst[j]->getCenter(CoordType::scene);
            QPointF pt2 = levLst[j+1]->getCenter(CoordType::scene);

            levLst[j]->setSwap(pt2);
            levLst[j+1]->setSwap(pt1);

            qreal sum1 = levLst[j]->getSumEdgesLength(false) + levLst[j+1]->getSumEdgesLength(false);
            qreal sum2 = levLst[j]->getSumEdgesLength(true) + levLst[j+1]->getSumEdgesLength(true);

            int newIntersect = levLst[j]->getIntersections(true);

            levLst[j]->dropSwap();
            levLst[j+1]->dropSwap();

            if (levLst[j]->getNodeRect(CoordType::scene).intersects(levLst[j+1]->getNodeRect(CoordType::scene))) {
                continue;
            }

            if (newIntersect < inter) {
                swapNodes(levLst[j], levLst[j+1]);
                //                qDebug()<<"swapReason:intersections"<<t.elapsed();
                return;
            } else if (newIntersect <= inter && (sum2 < sum1 && sum1-sum2 > sum1*0.2)) {
                swapNodes(levLst[j], levLst[j+1]);
                //                qDebug()<<"swapReason:length"<<t.elapsed();
                return;
            }
        }
    }

    lockForce = false;

    //    qDebug()<<"noSwap"<<t.elapsed();
}

void TermGroup::animateGroup()
{
    //    if(animGrp.state() != QAbstractAnimation::Stopped)
    //        return;
    if (lockForce) {
        return;
    }

    for (GraphicItemTerm* node : nodesList) {
        if (animGrp.state() != QAbstractAnimation::Stopped && node->getPaintLevel() == currAnimLevel) {
            continue;
        }
        node->countForces();
    }

    bool someMoved = false;
    for (GraphicItemTerm* node : nodesList) {
        if (animGrp.state() != QAbstractAnimation::Stopped && node->getPaintLevel() == currAnimLevel) {
            continue;
        }
        if (node->applyMove() && !someMoved) {
            someMoved = true;
        }
    }

    if (!someMoved) {
        stopAnimation();
//        animTimer.stop();
    }
}

void TermGroup::hideRect(QGraphicsRectItem *item)
{
    item->setBrush(AppStyle::Colors::transparent);
    item->setPen(QPen(AppStyle::Colors::transparent));
}

GraphicItemTerm::List TermGroup::getRootNodes()
{
    GraphicItemTerm::List ret;
    for (GraphicItemTerm* node : nodesList) {
        if (node->isRoot()) {
            ret << node;
        }
    }
    return ret;
}

GraphicItemTerm::List TermGroup::getNodesInLevel(int lev) const
{
    GraphicItemTerm::List ret;
    for (GraphicItemTerm* node : nodesList) {
        if (node->getPaintLevel() == lev) {
            ret << node;
        }
    }

    // sort
    int nMin;

    for (int i = 0; i < ret.size(); i++) {
        nMin = i;
        for (int j = i + 1; j < ret.size(); j++) {
            if (ret[j]->pos().y() < ret[nMin]->pos().y()) {
                nMin = j;
            }
        }
        ret.swap(i, nMin);
    }
    return ret;
}

GraphicItemTerm::List TermGroup::getOrphanNodes()
{
    GraphicItemTerm::List ndLst;
    for (GraphicItemTerm* n : nodesList) {
        if (n->isOrphan()) {
            ndLst << n;
        }
    }

    return ndLst;
}

GraphicItemTerm::List TermGroup::getInTreeNodes()
{
    GraphicItemTerm::List ret;
    for (GraphicItemTerm* n : nodesList) {
        if (n->isInTree()) {
            ret << n;
        }
    }
    return ret;
}

qreal TermGroup::getGroupMinWidth()
{
    qreal width = 0.0;

    qreal groupNameWidth = grNmItem->getNameRect().width();
    qreal treeWidth = getTheoreticalTreeSize().width();
    qreal orphansWidth = getVerticalStackedSize(getOrphanNodes()).width();

    width = qMax(width, groupNameWidth);
    width = qMax(width, treeWidth);
    width = qMax(width, orphansWidth);

    return width;
}

GraphicItemTerm::List TermGroup::getAllNodes()
{
    return nodesList;
}

EdgesList TermGroup::getAllEdges()
{
    return edgesList;
}

void TermGroup::setAnimSpeed(int val)
{
    checkSwapTimer.setInterval(static_cast<int>(val*1.5));
    swAnim1.setDuration(val);
    swAnim2.setDuration(val);
    animSpeed = val;
}

int TermGroup::getAnimSpeed()
{
    return animSpeed;
}

void TermGroup::setBasePoint(QPointF pt)
{
    baseRect->setPos(pt);
}

void TermGroup::updateRectsPositions()
{
    qreal vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal hSpacer = AppStyle::Sizes::groupHorizontalSpacer;
    QPointF basePoint(QPointF(hSpacer, vSpacer));

    QSizeF nameSize = grNmItem->getNameRect().size();
    QSizeF treeSize = getTheoreticalTreeSize();
    QSizeF orphansSize = getOrphansSize();

    // Устанавливаем базовую точку имени
    grNmItem->setPos(basePoint);
    basePoint.ry() += nameSize.height() + vSpacer;

    // Вычисляем под дерево
    treeRect->setPos(basePoint);
    treeRect->setRect(QRectF(QPoint(), treeSize));  // Применяем его
    if (getTheoreticalTreeSize().height() > 0)
        basePoint.ry() += getTheoreticalTreeSize().height() + vSpacer;

    // Вычисляем под несвязанные вершины
    orphansRect->setPos(basePoint);
    orphansRect->setRect(QRectF(QPoint(), orphansSize));  // Применяем
}

void TermGroup::updateBaseRectSize()
{
    QSizeF nameSize = grNmItem->getNameRect().size();
    QSizeF treeSize = getTheoreticalTreeSize();
    QSizeF orphansSize = getOrphansSize();
    qreal vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal hSpacer = AppStyle::Sizes::groupHorizontalSpacer;

    qreal width = 0.0;
    qreal height = 0.0;

    width = qMax(width, nameSize.width());
    width = qMax(width, treeSize.width());
    width = qMax(width, orphansSize.width());
    width += hSpacer*2;

    height += vSpacer;
    height += nameSize.height();
    if (treeSize.height() > 0)
        height += treeSize.height() + vSpacer;
    if (orphansSize.height() > 0)
        height += orphansSize.height() + vSpacer;
    height += vSpacer;

    baseRect->setRect(QRectF(QPointF(), QSizeF(width, height)));
}

void TermGroup::updateGroupFrame()
{
    updateRectsPositions();
    updateBaseRectSize();
}

EdgesList TermGroup::searchConnections()
{
    EdgesList ret;
    for (GraphicItemTerm* n : nodesList) {
        for (GraphicItemTerm* m : nodesList) {
            if (n == m) {
                continue;
            }
            QStringList lst = n->getTags();
            for (QString s : lst) {
                for (QString z : m->getNameFormList()) {  // TODO: Переименовать при случае
                    QString s1 = z.toLower();
                    QString s2 = s.toLower();
                    if (s1 == s2) {  // Точное соответствие
                        ret << addNewEdge(m, n);
                    }
                    int acceptableDistance = 4*(1 + s1.count(' '));  // Пропорционально количеству слов
                    acceptableDistance = 4;
                    // TODO: Сделать защиту от формирования двухсторонних связей
                    // TODO: Найти способ вызывать функцию в mainScene addEdge
                    // TODO: Переделать так чтобы это было предложением а не обязательным действием
                    if (TagProcessor::getLevDistance(s1, s2) <= acceptableDistance) {
                        if (s1.left(3) == s2.left(3)) {
                            ret << addNewEdge(m, n);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

int TermGroup::getLayersCount() const
{
    int ret = 0;
    for (GraphicItemTerm* node : nodesList) {
        ret = qMax(ret, node->getPaintLevel());
    }

    return ret;
}

qreal TermGroup::getMaxHeightInAllLevels() const
{
    qreal maxHeight = 0.0;
    for (int i = 0; i <= getLayersCount(); i++) {
        QSizeF stackSize = getVerticalStackedSize(getNodesInLevel(i));
        maxHeight = qMax(maxHeight, stackSize.height());
    }
    return maxHeight;
}

void TermGroup::setOrphCoords(qreal maxWidth)
{
    GraphicItemTerm::List nodesList = getOrphanNodes();
    if (nodesList.isEmpty()) {
        return;
    }

    for (int i = 0; i < nodesList.size(); i++) {
        int nMax = i;
        for (int j = i+1; j < nodesList.size(); j++) {
            qreal currentWidth = nodesList[j]->getFrameRect(CoordType::zeroPoint).size().width();
            qreal maxWidth = nodesList[nMax]->getFrameRect(CoordType::zeroPoint).size().width();
            if (currentWidth < maxWidth) {
                nMax = j;
            }
        }
        nodesList.swap(i, nMax);
    }

    int x = 0, y = 0;
    qreal maxHeightInRow = 0.0;
    qreal groupMinWidth = getGroupMinWidth();
    maxWidth = qBound(10.0, groupMinWidth, 700.0);

    for (GraphicItemTerm* currNode : nodesList) {
        QSizeF nodeSize = currNode->getNodeRect(CoordType::zeroPoint).size();

        if (x + nodeSize.width() > maxWidth) {
            y+= maxHeightInRow + AppStyle::Sizes::orphansVerticalSpacer;
            //Переходим на следующий ряд
            maxHeightInRow = 0;
            x = 0;
        }

        currNode->setPos(x, y);
        x += nodeSize.width() + AppStyle::Sizes::orphansHorizontalSpacer;

        maxHeightInRow = qMax(maxHeightInRow, nodeSize.height());
    }
}

void TermGroup::setLayers()
{
    for (GraphicItemTerm* node : getRootNodes()) {
        node->setLevel(0);
    }
}

void TermGroup::setTreeCoords()
{
    int maxLevel = getLayersCount();

    if (nodesList.isEmpty()) {
        return;
    }

    qreal x = 0, y = 0;
    qreal layerHeight = 0;
    qreal layerWidth = 0;
    qreal allTreeHeight = getMaxHeightInAllLevels();

    for (int i = 0; i <= maxLevel; i++) {
        QList< GraphicItemTerm *> tList = getNodesInLevel(i);

        // Сортируем вершины в более "удачном" порядке
        tList = sortNodesInLayer(tList);

        layerHeight = getVerticalStackedSize(tList).height();
        layerWidth = getVerticalStackedSize(tList).width();

        y = -(allTreeHeight - layerHeight)/2.0;
        x += layerWidth/2;  // Сначала добавляем первую половину максимума

        for (GraphicItemTerm* node : tList) {
            node->setPos(x - node->getNodeRect(CoordType::scene).width()/2, -y);
            y -= node->getFrameRect(CoordType::zeroPoint).size().height();
        }

        //А после выставления всех координат - вторую
        x += (layerWidth/2 + AppStyle::Sizes::treeLayerHorizontalSpacer);
    }
}

GraphicItemTerm::List TermGroup::sortNodesInLayer(GraphicItemTerm::List lst)
{
    // Сначала сортируем по убыванию количества связей
    GraphicItemTerm::List ret;
    int nMax = 0;

    for (int i = 0; i < lst.size(); i++) {
        nMax = i;
        for (int j = i + 1; j < lst.size(); j++) {
            if (lst[j]->getUpLevels() > lst[nMax]->getUpLevels()) {
                nMax = j;
            }
        }
        lst.swap(i, nMax);
    }
    // Формируем новый список так чтобы максимально нагруженные вершины были ближе к центру
    for (int i = 0; i < lst.size(); i++) {
        if (i%2 == 0)
            ret.push_front(lst[i]);
        else
            ret.push_back(lst[i]);
    }
    return ret;
}

Edge *TermGroup::addNewEdge(GraphicItemTerm *node1, GraphicItemTerm *node2)
{
    Edge* edge = new Edge(node1, node2);
    node1->addEdgeRef(edge);
    node2->addEdgeRef(edge);
    return edge;
}

void TermGroup::setNeighbours()
{
    QList<int> levLst;

    for (GraphicItemTerm* node : nodesList) {
        if (!levLst.contains(node->getPaintLevel())) {
            levLst << node->getPaintLevel();
        }
    }

    QList<GraphicItemTerm*> levNd;
    for (int i : levLst) {
        levNd.clear();

        for (GraphicItemTerm *node : nodesList) {
            if (node->getPaintLevel() == i) {
                levNd << node;
            }
        }

        for (int j = 0; j< levNd.size(); j++) {
            levNd[j]->clearNeighbours();
            for (int k = 0; k < levNd.size(); k++) {
                if (j == k) {
                    continue;
                }
                levNd[j]->addToNeighboursList(levNd[k]);
            }
        }
    }
}

QString TermGroup::getName()
{
    return grNmItem->getNameOnly();
}

QUuid TermGroup::getUuid()
{
    return groupUuid;
}

QSizeF TermGroup::getOrphansSize()
{
    QRectF orphansRc;
    for (GraphicItemTerm* node : getOrphanNodes()) {
        orphansRc = orphansRc.united(node ->getNodeRect(CoordType::scene));
    }
    return orphansRc.size();
}

QSizeF TermGroup::getTheoreticalTreeSize()
{
    int layers = getLayersCount();
    qreal treeWidth = 0.0;
    qreal treeHeight = 0.0;

    for (int layer = 0; layer <= layers; layer++) {
        GraphicItemTerm::List nodes = getNodesInLevel(layer);
        QSizeF levelSize = getVerticalStackedSize(nodes);
        treeWidth += levelSize.width();
        if (layer < layers) {
            treeWidth += AppStyle::Sizes::treeLayerHorizontalSpacer;
        }
        treeHeight = qMax(treeHeight, levelSize.height());
    }
    return QSizeF(treeWidth, treeHeight);
}
