#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_def.h"
#include "radpat.h"
#include "misura.h"
#include "motor.h"

#include <iostream>
#include <QStringList>
#include <QPalette>
#include <QFileDialog>
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCharts/QChartView>
#include <QThread>
#include <QPushButton>



using namespace QtCharts;

QThread *tt_mis;
QThread *tt_motor;

Misura *mis;
Motor  *mot;


double yMin = 0;
double yMax = 10;

double xMin = 0;
double xMax = 10;


extern motor_par mot1;
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //setting up the main interface
    ui->setupUi(this);
    graph_show = true;
    measure_number = 0;
    compl_perc = 0;

    //setting up the measure thread
    tt_mis   = new QThread();
    tt_motor = new QThread();
    mis = new Misura();
    mot = new Motor();

    mis->moveToThread(tt_mis);
    mot->moveToThread(tt_motor);


    ui->text_xmax->setDisabled(true);
    ui->text_xmin->setDisabled(true);
    ui->text_ymax->setDisabled(true);
    ui->text_ymin->setDisabled(true);

    connect(tt_mis,SIGNAL(started()),mis,SLOT(perform_measure()));
    connect(tt_motor, SIGNAL(started()),mot,SLOT(perform_motion()));
    connect(mot,SIGNAL(GPIBconnected()),this,SLOT(connectionGPIB()));

    connect(ui->btn_openGPIB,SIGNAL(clicked(bool)),this,SLOT(start_motor_thread()));



    connect(ui->start_meas,SIGNAL(clicked(bool)),this,SLOT(start_meas_thread()));
    connect(ui->refresh, SIGNAL(clicked(bool)), this, SLOT(refresh_data()));

    connect(mis,SIGNAL(start()),this, SLOT(start_mis()));
    connect(mis,SIGNAL(update_progress()),this,SLOT(update_prog()));
    connect(mis,SIGNAL(finished()),this, SLOT(end_mis()));


    connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(set_overwrite_graph()));
    connect(ui->rdb_polar,SIGNAL(clicked(bool)),this,SLOT(polar_plot()));
    connect(ui->rdb_scat,SIGNAL(clicked(bool)),this,SLOT(scatter_plot()));
    //connect(ui->update_ranges,SIGNAL(clicked(bool)),this,SLOT(updateRanges()));



    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(menuExit()));

    connect(ui->Load_cut, SIGNAL(clicked(bool)), this, SLOT(load_cut()));

    connect(ui->text_xmin,SIGNAL(textChanged()),this,SLOT(updateRanges()));
    connect(ui->text_xmax,SIGNAL(textChanged()),this,SLOT(updateRanges()));
    connect(ui->text_ymin,SIGNAL(textChanged()),this,SLOT(updateRanges()));
    connect(ui->text_ymax,SIGNAL(textChanged()),this,SLOT(updateRanges()));

    connect(ui->checkBox_2, SIGNAL(clicked(bool)),this,SLOT(activateManualRanges()));




    measure->setName("Radiation Pattern");
    measure_spline->setName("Spline");


    for (int i = -90; i <= 90; i += 10){
        measure->append(i, i);
        measure_spline->append(i,i);
    }




    //setting up radiation pattern plot canvas

    chartp = new QPolarChart();
    chartl = new QChart();
    view = new QChartView();
    radialAxis = new QValueAxis();
    angularAxis = new QValueAxis();

    chartl->addSeries(measure_spline);
    chartl->addSeries(measure);

    measure->clear();
    measure_spline->clear();
    chartl->createDefaultAxes();
    view->setChart(chartl);


    connect(measure, &QScatterSeries::pointAdded,[=](int index){
        qreal y = measure->at(index).y();
        if(y<yMin || y > yMax){
            if(y<yMin) yMin = y;
            if(y>yMax) yMax = y;
            chartl->axisY()->setRange(yMin-5,yMax+5);
            view->update();
        }});
    connect(measure, &QScatterSeries::pointAdded,[=](int index){
        qreal x = measure->at(index).x();
        if(x < xMin || x > xMax){
            if(x < xMin) xMin = x;
            if(x > xMax) xMax = x;
            chartl->axisX()->setRange(xMin-5,xMax+5);
            view->update();
        }});

    connect(measure_spline, &QSplineSeries::pointAdded,[=](int index){
        qreal y = measure_spline->at(index).y();
        if(y<yMin || y > yMax){
            if(y<yMin) yMin = y;
            if(y>yMax) yMax = y;
            chartl->axisY()->setRange(yMin-5,yMax+5);
            view->update();
        }});
    connect(measure_spline, &QSplineSeries::pointAdded,[=](int index){
        qreal x = measure_spline->at(index).x();
        if(x < xMin || x > xMax){
            if(x < xMin) xMin = x;
            if(x > xMax) xMax = x;
            chartl->axisX()->setRange(xMin-5,xMax+5);
            view->update();
        }});
    //setting up the radiation pattern interface
    g_rad.setFixedSize(600,480);
    g_rad.setCentralWidget(view);
    g_rad.show();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_cut(){
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/home/stefanomandelli/CUTS",tr("*.cut"));
    ui->listWidget->addItems(fileNames);
}






void MainWindow::start_meas_thread()
{
    if(measure_number==0){
        tt_mis->start();
    }else{


        measure = new QScatterSeries;
        //measure_spline = new QSplineSeries;
        for (int i = -90; i <= 90; i += 10){
            measure->append(i, i);
            measure->clear();

        }
        chartl->addSeries(measure);
        chartl->createDefaultAxes();
        view->setChart(chartl);
        tt_mis->start();

    }
}


void MainWindow::refresh_data()
{
    measure_spline->clear();
    measure->clear();
    view->update();
}



void MainWindow::start_mis()
{
    ui->meas_log->setText("Grabbing Data");
}

void MainWindow::update_prog()
{
    ui->progressBar->setValue(compl_perc);
}

void MainWindow::end_mis()
{
    ui->meas_log->setText("Measure N."+QString::number(measure_number)+"\nCompleted.");
    tt_mis->exit();
}


void MainWindow::set_overwrite_graph()
{
    if(ui->checkBox->isChecked()){
        graph_show = true;
    }else{
        graph_show = false;
    }
}


void MainWindow::scatter_plot()
{


    //chartl->addSeries(measure);
    //chartl->createDefaultAxes();
    view->setChart(chartl);
    view->update();

}


void MainWindow::polar_plot()
{
    chartp->addSeries(measure);
    chartp->createDefaultAxes();
    view->setChart(chartp);
}


void MainWindow::menuExit()
{
    delete chartl;
    delete chartp;
    g_rad.close();
    this->close();
}

void MainWindow::updateRanges()
{
    //std::cout << tt_mis->currentThreadId() << std::endl;
    if(ui->text_xmax->toPlainText() != "" || ui->text_xmin->toPlainText() != "" ) chartl->axisX()->setRange(ui->text_xmin->toPlainText().toDouble(), ui->text_xmax->toPlainText().toDouble());
    if(ui->text_ymax->toPlainText() != "" || ui->text_ymin->toPlainText() != "" ) chartl->axisY()->setRange(ui->text_ymin->toPlainText().toDouble(), ui->text_ymax->toPlainText().toDouble());
    view->update();

}

void MainWindow::activateManualRanges()
{
    if(ui->checkBox_2->isChecked()){
        ui->text_xmin->setEnabled(true);
        ui->text_xmax->setEnabled(true);
        ui->text_ymin->setEnabled(true);
        ui->text_ymax->setEnabled(true);
    }else{
        ui->text_xmax->setDisabled(true);
        ui->text_xmin->setDisabled(true);
        ui->text_ymax->setDisabled(true);
        ui->text_ymin->setDisabled(true);
    }
}

void MainWindow::start_motor_thread()
{
    tt_motor->start();
}
void MainWindow::connectionGPIB()
{
    QPalette palette;
    palette.setBrush( QPalette::Background, QBrush( Qt::green ) );
    ui->labelStatus->setPalette(palette);

}
