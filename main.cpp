#include "mainwindow.h"
#include <QApplication>
#include "user_def.h"
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCharts/QChartView>

using namespace QtCharts;



MainWindow *ptr_mainwindow;
motor_par mot1;
QScatterSeries *measure;
QSplineSeries *measure_spline;
QPolarChart *chartp;
QChart *chartl;
QChartView *view;
QValueAxis *radialAxis;
QValueAxis *angularAxis;

bool graph_show;
int  measure_number;
double compl_perc;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    measure = new QScatterSeries;
    measure_spline = new QSplineSeries;
    ptr_mainwindow = new MainWindow;
    ptr_mainwindow->setFixedSize(640,640);
    ptr_mainwindow->show();

    return a.exec();
}
