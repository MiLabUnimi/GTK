#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCharts/QChartView>

#include "radpat.h"

using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    RadPat g_rad;



private slots:
    void menuExit();
    void updateRanges();

    void load_cut();
    void start_meas_thread();
    void refresh_data();
    void start_mis();
    void end_mis();
    void set_overwrite_graph();
    void update_prog();

    void scatter_plot();
    void polar_plot();

    void activateManualRanges();

    void connectionGPIB();
    void start_motor_thread();
};

#endif // MAINWINDOW_H
