#include "termgroup.h"

int TermGroup::animSpeed = 300;

TermGroup::TermGroup(QSqlRecord rec, QObject *parent):
    QObject(parent),
    TermGroupInfo()
{
    QString groupName = rec.value(TermGroupColumn::name).toString();
    this->grNmItem = new LabelGraphicItem(groupName);

    setGroupUuid(QUuid(rec.value(TermGroupColumn::longUID).toString()));
    setType(static_cast<GroupType>(rec.value(TermGroupColumn::type).toInt()));

//    qDebug() << "Create group" << getName();
}

TermGroup::TermGroup(QJsonDocument doc, QObject *parent):
    QObject(parent),
    TermGroupInfo()
{
    QJsonObject jsonObject = doc.object();

    setGroupUuid(QUuid(jsonObject.value("longUID").toString()));
    setType(static_cast<GroupType>(jsonObject.value("type").toInt()));

    QString groupName = jsonObject.value("name").toString();
    this->grNmItem = new LabelGraphicItem(groupName);

//    qDebug() << "Create group" << getName();
}

TermGroup::~TermGroup()
{
//    qDebug() << "Delete group" << getName();
    checkSwapTimer.stop();
    animTimer.stop();

    delete grNmItem;
    delete orphansRect;
    for (auto child : baseRect->childItems()) {
        child->setParentItem(nullptr);
    }
    delete baseRect;
}

void TermGroup::initNewNodes()
{
    loadEdges();
    removeCycles();
    removeExceedEdges();

    baseRect = new QGraphicsRectItem(QRectF(QPoint(0, 0), QSizeF(10.0, 10.0)), nullptr);
    baseRect->setZValue(1);

    orphansRect = new RectGraphicItem();
    orphansRect->setParentItem(baseRect);

    //    centerRect  = new QGraphicsRectItem( nullptr );

    baseRect->setBrush(getGroupFillColor());
    baseRect->setPen(getGroupColor());

    this->grNmItem->setParentItem(baseRect);

    setLevels();
    initTrees();
    addTreeRectsToScene();

    addOrphansToParents();
    addEdgesToParents();

    setTreeCoords();
    setNeighbours();
    setOrphCoords();

    checkSwapTimer.setSingleShot(false);
    setAnimSpeed(300);
    connect(&checkSwapTimer, SIGNAL(timeout()), SLOT(checkSwap()));

    animTimer.setSingleShot(false);
    animTimer.setInterval(50);
    connect(&animTimer, SIGNAL(timeout()), SLOT(animateGroup()));
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
    ret[GroupType::freeEdges] = "Свободные связи";
    ret[GroupType::terms]     = "Термины";
    return ret;
}

void TermGroup::loadNodes(GraphicItemTerm::List newNodes)
{
    clearNodesList();
    for (GraphicItemTerm* node : newNodes) {
        if (node->getGroupUuid() != this->getUuid()) {
            qDebug() << "NodeLoad error for node:" << node->getUuid().toString();
            continue;
        }
        addNodeToList(node);
    }
    initNewNodes();
}

bool TermGroup::needPaint() const
{
    for (auto node : getAllNodes()) {
        if (node->needPaint) {
            return true;
        }
    }

    for (auto edge : getAllEdges()) {
        if (edge->needPaint) {
            return true;
        }
    }

    return false;
}

void TermGroup::resetPaintFlags()
{
    for (auto node : getAllNodes()) {
        node->needPaint = true;
    }

    for (auto edge : getAllEdges()) {
        edge->needPaint = true;
    }
}

GraphicItemTerm *TermGroup::getNodeAtPoint(const QPointF& pt) const
{
    for (auto tree : trees) {
        if (tree->getTreeRect(CoordType::scene).contains(pt)) {
            return tree->getNodeAtPoint(pt);
        }
    }

    auto orphansRect = QRectF(this->orphansRect->scenePos(), this->orphansRect->size());
    if (orphansRect.contains(pt)) {
        for (auto orphan : getOrphanNodes()) {
            if (orphan->getNodeRect(CoordType::scene).contains(pt)) {
                return orphan;
            }
        }
    }

    return nullptr;
}

void TermGroup::setHover(QPointF mousePos)
{
    for (auto tree : trees) {
        tree->checkHover(mousePos);
    }

    for (auto orphan : getOrphanNodes()) {
        auto hover = orphan->getNodeRect(CoordType::scene).contains(mousePos);
        orphan->setHover(hover);
    }
}

void TermGroup::addOrphansToParents()
{
    for (GraphicItemTerm* node : getOrphanNodes()) {
        node->setParentItem(orphansRect);
    }
}

void TermGroup::addEdgesToParents()
{
    for (Edge* edge : getAllEdges()) {
        for (auto tree : trees) {
            if (tree->hasEdge(edge)) {
                edge->setSceneParent(tree->rect);
            }
        }
    }
}

QJsonDocument TermGroup::getJsonDoc()
{
    QJsonDocument doc;
//    obj.insert("uid",     QJsonValue(grUid));
    QJsonObject obj;
    obj.insert(TermGroupColumn::longUID, QJsonValue(getUuid().toString()));
    obj.insert(TermGroupColumn::name, QJsonValue(getName()));
    obj.insert(TermGroupColumn::type, QJsonValue(static_cast<int>(this->getType())));

    QJsonArray ndArr;

    for (GraphicItemTerm* node : getAllNodes()) {
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
}

void TermGroup::stopAnimation()
{
    animTimer.stop();
    checkSwapTimer.stop();
}

void TermGroup::sceneUpdateSignal()
{
    updateGroupFrame();
}

void TermGroup::checkSwap()
{
    for (auto tree : trees) {
        tree->checkSwap();
    }
}

void TermGroup::animateGroup()
{
    for (auto tree : trees) {
        tree->animateTree();
    }
}

void TermGroup::addTreeRectsToScene()
{
    for (auto tree : trees) {
        tree->rect->setParentItem(baseRect);
    }
}

qreal TermGroup::getGroupMinWidth()
{
    qreal width = 0.0;

    qreal groupNameWidth = grNmItem->getNameSize().width();
    qreal treeWidth = getAllTreesSize().width();
    qreal orphansWidth = NodeVerticalStackTools::getNodeVerticalStackedSize(getOrphanNodes()).width();

    width = qMax(width, groupNameWidth);
    width = qMax(width, treeWidth);
    width = qMax(width, orphansWidth);

    return width;
}

void TermGroup::setAnimSpeed(int val)
{
    checkSwapTimer.setInterval(static_cast<int>(val*1.5));
    for (auto tree : trees) {
        tree->swAnim1.setDuration(val);
        tree->swAnim2.setDuration(val);
    }
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

    auto nameSize = grNmItem->getNameSize();

    // Устанавливаем базовую точку имени
    grNmItem->setPos(basePoint);
    basePoint.ry() += nameSize.height() + vSpacer;

    // Вычисляем под дерево
    for (auto tree : trees) {
        auto treeSize = tree->getTreeSize();
        tree->rect->setPos(basePoint);
        tree->rect->setRect(QRectF(QPointF(), treeSize));
        basePoint.ry() += treeSize.height() + vSpacer;
    }

    QSizeF orphansSize = getOrphansSize();
    // Вычисляем под несвязанные вершины
    orphansRect->setPos(basePoint);
    orphansRect->setSize(orphansSize);  // Применяем
}

void TermGroup::updateBaseRectSize()
{
    QSizeF nameSize = grNmItem->getNameSize();
    QSizeF treesSize = getAllTreesSize();
    QSizeF orphansSize = getOrphansSize();
    qreal vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal hSpacer = AppStyle::Sizes::groupHorizontalSpacer;

    qreal width = 0.0;
    qreal height = 0.0;

    width = qMax(width, nameSize.width());
    width = qMax(width, treesSize.width());
    width = qMax(width, orphansSize.width());
    width += hSpacer*2;

    height += vSpacer;
    height += nameSize.height();
    if (treesSize.height() > 0)
        height += treesSize.height() + vSpacer;
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

void TermGroup::setTreeCoords()
{
    for (auto tree : trees) {
        tree->setTreeNodeCoors();
    }
}

QString TermGroup::getName()
{
    return grNmItem->getLabel();
}

QPointF TermGroup::getNamePos()
{
    auto pt = grNmItem->scenePos();
    pt.ry() += grNmItem->getNameSize().height() / 2;
    return pt;
}

QRectF TermGroup::getGroupRect() const
{
    return baseRect->rect().translated(baseRect->scenePos());
}

QColor TermGroup::getGroupColor()
{
    return AppStyle::Colors::Groups::border;
}

QColor TermGroup::getGroupFillColor()
{
    switch (getType()) {
    case GroupType::freeEdges: return AppStyle::Colors::Groups::backgroundFreeConnections;
    case GroupType::terms: return AppStyle::Colors::Groups::backgroundTerms;
    }
}
