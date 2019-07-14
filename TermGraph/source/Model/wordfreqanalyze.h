#pragma once

#include <QWidget>
//#include <QtCharts/QChartView>
//#include <QtCharts/QChart>
//#include <QtCharts/QBarSeries>
//#include <QtCharts/QBarSet>
//#include <QtCharts/QBarCategoryAxis>


namespace Ui {
class WordFreqAnalyze;
}

using WrdStat = QMap<QString, int>;

class WordFreqAnalyze : public QWidget
{
    Q_OBJECT

public:
    explicit WordFreqAnalyze(QWidget *parent = nullptr);
    ~WordFreqAnalyze();

//    WrdStat makeWordStat( QString txt );

private slots:

//    void makeAnalyze();

private:

//    QtCharts::QChart*     chart;
//    QtCharts::QChartView* view;

//    QString dropSideSyms(QString str);

    Ui::WordFreqAnalyze *ui;
};
