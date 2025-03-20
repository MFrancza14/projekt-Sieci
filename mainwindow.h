#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "App.h"
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "arxwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_START_clicked();
    void on_STOP_clicked();

    void on_pushButton_clicked();

    void on_ODCZYT_clicked();
    void updateChart();
    void on_RESET_clicked();
    void on_POMOC_clicked();

    void on_BTNUSTAWALL_clicked();

    void on_BTNZAPISZCONF_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    void setUIFromApp();
    void clearAllSeries();
    void resetAxes();
    Ui::MainWindow *ui;
    App* app;
    QChart *chart;
    QChartView *chartView;
    QChart *chart1;
    QChartView *chartView1;
    QValueAxis *axisX1;
    QValueAxis *axisY1;
    QLineSeries *seriesUi;        //y
    QLineSeries *seriesUp;         //u
    QLineSeries *seriesUd;
    QLineSeries *seriesUchyb;

    QLineSeries *seriesY;        //y
    QLineSeries *seriesU;         //u
    QLineSeries *seriesW;

    QValueAxis *axisX;
    QValueAxis *axisY;
    QTimer *timer;        //timer
    ARXwindow *arx_window;
    int currentX;            //
};
#endif // MAINWINDOW_H
