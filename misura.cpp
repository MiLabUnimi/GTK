#include "misura.h"
#include "unistd.h"
#include <iostream>

#include <ctime>
#include <QString>
#include <QThread>
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCharts/QChartView>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace QtCharts;

extern MainWindow *ptr_mainwindow;

extern QScatterSeries *measure;
extern QSplineSeries *measure_spline;
extern QPolarChart *chartp;
extern QChart *chartl;
extern QChartView *view;
extern QValueAxis *radialAxis;
extern QValueAxis *angularAxis;

extern bool graph_show;
extern int measure_number;
extern double compl_perc;

Misura::Misura()
{

}

Misura::~Misura()
{
    delete this;
}


void Misura::perform_measure()
{
    srand((unsigned)time(NULL));
    if(measure_number==0){
        emit start();
        double N_point = 100.0;
        if(!graph_show){
            measure->clear();
            measure_spline->clear();
        }

            for(int i=0; i<100; i++){
                compl_perc = ((double(i)+1.0)/N_point)*100;
                emit update_progress();
                measure->append(i*7, double(i)*100.0*rand()/double(RAND_MAX) );
                measure_spline->append(i*7, double(i)*100.0*rand()/double(RAND_MAX) );
                view->update();
                usleep(15000);
            }
            emit finished();
    }else{

        emit start();
        double N_point = 100.0;
        if(!graph_show){
            measure->clear();
            //measure_spline->clear();
        }

            for(int i=0; i<100; i++){
                compl_perc = ((double(i)+1.0)/N_point)*100;
                emit update_progress();
                measure->setName("Radiation Pattern N."+QString::number(measure_number));
                //measure_spline->setName("Radiation Pattern Spline N."+QString::number(measure_number));
                measure->append(i*(7+measure_number*10), double(i)*100.0*rand()/double(RAND_MAX));
                //measure_spline->append(i*(7+measure_number*10), double(i)*100.0*rand()/double(RAND_MAX));

                view->update();
                usleep(15000);
            }
            emit finished();

    }
    measure_number++;

}
