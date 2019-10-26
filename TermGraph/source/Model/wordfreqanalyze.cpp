/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "wordfreqanalyze.h"
#include "ui_wordfreqanalyze.h"

WordFreqAnalyze::WordFreqAnalyze(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordFreqAnalyze)
{
    ui->setupUi(this);

//    view  = new QtCharts::QChartView();
//    chart = new QtCharts::QChart();

//    chart->setTitle("Частота встречаемости слов");
//    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
//    chart->legend()->hide();

//    view->setChart(chart);

//    ui->statLayout->addWidget( view );

//    connect(ui->analyzeBtn,SIGNAL(clicked(bool)),SLOT(makeAnalyze()));

}

WordFreqAnalyze::~WordFreqAnalyze()
{
    delete ui;
}

//WrdStat WordFreqAnalyze::makeWordStat( QString txt )
//{
//    txt = dropSideSyms( txt );
//    QStringList txtList = txt.split(' ',QString::SkipEmptyParts);
//    WrdStat words;
//    for( QString &s : txtList ) {
//        if( words.contains(s) )
//            words[s] = words[s] + 1;
//        else
//            words[s] = 1;
//    }
//    return words;
//}

//void WordFreqAnalyze::makeAnalyze()
//{
//    WrdStat words = makeWordStat( ui->text->toPlainText() );
//    chart->removeAllSeries();

//    QStringList categories;
//    for( QString &s : words.keys() ) {
//        QtCharts::QBarSet *set0 = new QtCharts::QBarSet(s);
//        set0->append( words[s] );
//        categories << s;
//        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
//        series->append( set0 );
//        chart->addSeries(series);
//    }


//    QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();

//    axis->append(categories);
//    chart->createDefaultAxes();
//    chart->setAxisX(axis, series);

//}

//QString WordFreqAnalyze::dropSideSyms(QString str)
//{
//    for(int i=0;i<str.size();i++) {
//        if( !( str[i].isLetter() || str[i] == ' ' ) ) {
//            str[i] = ' ';
//        }
//    }
//    return str;
//}


/*
 *

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//![1]
    QBarSet *set0 = new QBarSet("Jane");
    QBarSet *set1 = new QBarSet("John");

    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
    *set1 << 5 << 0 << 0 << 4 << 0 << 7;
//![1]

//![2]
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);

//![2]

//![3]
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple barchart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
//![3]

//![4]
    QStringList categories;
    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
//![4]

//![6]
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
//![6]

//![7]
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(420, 300);
    window.show();
//![7]

    return a.exec();
}
*/
