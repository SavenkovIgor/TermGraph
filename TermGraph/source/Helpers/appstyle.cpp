#include "appstyle.h"

// Nodes
QColor AppStyle::Colors::Nodes::text = QColor(255, 255, 255);
QColor AppStyle::Colors::Nodes::border = QColor(0, 0, 0);

QColor AppStyle::Colors::Nodes::leaf = QColor(109, 154, 40);
QColor AppStyle::Colors::Nodes::root = QColor(128, 41, 21);
QColor AppStyle::Colors::Nodes::orphan = QColor(18, 75, 106);

QColor AppStyle::Colors::Nodes::leafSelected = QColor(131, 195, 27);
QColor AppStyle::Colors::Nodes::rootSelected = QColor(220, 76, 29);
QColor AppStyle::Colors::Nodes::orphanSelected = QColor(11, 119, 177);

QColor AppStyle::Colors::Nodes::atLearn = QColor(245, 222, 179);

QColor AppStyle::Colors::Groups::border = QColor(255, 255, 255);
QColor AppStyle::Colors::Groups::text = QColor(255, 255, 255);
QColor AppStyle::Colors::Groups::backgroundTerms = QColor(0, 0, 0, 0);
QColor AppStyle::Colors::Groups::backgroundFreeConnections = QColor(0, 0, 0, 0);

QColor AppStyle::Colors::Edges::standard = QColor(Qt::black);
QColor AppStyle::Colors::Edges::termin = QColor(34, 91, 122);
QColor AppStyle::Colors::Edges::description = QColor(Qt::green);
QColor AppStyle::Colors::Edges::selected = QColor(0, 166, 147);

QColor AppStyle::Colors::Scene::background = QColor(51, 47, 48);

QColor AppStyle::Colors::transparent = QColor(0,0,0,0);
QColor AppStyle::Colors::testColor = QColor(0, 255, 0);

qreal AppStyle::Sizes::groupVerticalSpacer = 10.0;
qreal AppStyle::Sizes::groupHorizontalSpacer = 10.0;

qreal AppStyle::Sizes::orphansVerticalSpacer = 10.0;
qreal AppStyle::Sizes::orphansHorizontalSpacer = 10.0;

qreal AppStyle::Sizes::treeLayerHorizontalSpacer = 15.0;

qreal AppStyle::Sizes::baseBlockWidth = 40.0;
