#ifndef WORDFREQANALYZE_H
#define WORDFREQANALYZE_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>


namespace Ui {
class WordFreqAnalyze;
}

typedef QMap<QString,int> WrdStat;

class WordFreqAnalyze : public QWidget
{
    Q_OBJECT

public:
    explicit WordFreqAnalyze(QWidget *parent = 0);
    ~WordFreqAnalyze();

    WrdStat makeWordStat( QString txt );

private slots:

    void makeAnalyze();

private:

    QtCharts::QChart*     chart;
    QtCharts::QChartView* view;

    QString dropSideSyms(QString str);

    Ui::WordFreqAnalyze *ui;
};

#endif // WORDFREQANALYZE_H
