#include "glb.h"

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

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
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
    return ptToStr(pt.x(), pt.y(), divider);
}

QString Glb::ptToStr(QPointF pt, QString divider){
    return ptToStr(pt.toPoint(), divider);
}

void Glb::saveFile(QString subDir, QString fileName, QString data) {
    QByteArray arr;
    arr.append(data);
    saveFile(subDir, fileName, arr);
}

void Glb::saveFile(QString subDir, QString fileName, QByteArray data)
{
    QDir groupsDir(qApp->applicationDirPath());
    groupsDir.mkpath(subDir);
    QFile someFile(subDir + "/" + fileName);
    if (someFile.open(QIODevice::WriteOnly)) {
        someFile.write(data);
        someFile.close();
    }
}

QString StdFolderPaths::groupsJsonPath()
{
    return QDir::home().absolutePath() + "/.TermGraph/GroupsJson";
}
