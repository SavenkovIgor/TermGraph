#include "glb.h"

qreal Sizes::groupVerticalSpacer = 10.0;
qreal Sizes::groupHorizontalSpacer = 10.0;

qreal Sizes::orphansVerticalSpacer = 10.0;
qreal Sizes::orphansHorizontalSpacer = 10.0;

qreal Sizes::treeLayerHorizontalSpacer = 15.0;

// Nodes
QColor Colors::Nodes::text = QColor(255, 255, 255);
QColor Colors::Nodes::border = QColor(0, 0, 0);

QColor Colors::Nodes::leaf = QColor("#6d9a28");
QColor Colors::Nodes::root = QColor("#802915");
QColor Colors::Nodes::orphan = QColor("#124b6a");

QColor Colors::Nodes::leafSelected = QColor("#83c31b");
QColor Colors::Nodes::rootSelected = QColor("#dc4c1d");
QColor Colors::Nodes::orphanSelected = QColor("#0b77b1");

QColor Colors::Groups::border = QColor(255, 255, 255);
QColor Colors::Groups::text = QColor(255, 255, 255);
QColor Colors::Groups::backgroundTerms = QColor(0, 0, 0, 0);
QColor Colors::Groups::backgroundFreeConnections = QColor(0, 0, 0, 0);

QColor Colors::Scene::background = QColor("#332f30");

QColor Colors::transparent = QColor(0,0,0,0);

quint16 NetworkSettings::listenPort = 46377;

CloudServicesWrapper*Glb::cloud = nullptr;
DBAbstract* Glb::db = nullptr;

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

void Glb::saveFile(QString subDir, QString fileName, QString data) {
    QByteArray arr;
    arr.append(data);
    saveFile(subDir,fileName,arr);
}

void Glb::saveFile(QString subDir, QString fileName, QByteArray data)
{
    QDir groupsDir(qApp->applicationDirPath());
    groupsDir.mkpath(subDir);
    QFile jsonFile(subDir + "/" + fileName);
    if(jsonFile.open(QIODevice::WriteOnly)) {
        jsonFile.write(data);
        jsonFile.close();
    }
}

QString StdFolderPaths::groupsJsonPath()
{
    return QDir::home().absolutePath() + "/.TermGraph/GroupsJson";
}

QFont Fonts::getFont()
{
    return qApp->font();
}

QFont Fonts::getWidthFont()
{
    QFont font = getFont();
    font.setWeight(2);
    return font;
}

QRectF Fonts::getTextMetrics(QString text, QFont font)
{
    QFontMetricsF mtr = QFontMetricsF(font);
    return  mtr.boundingRect(text);
}
