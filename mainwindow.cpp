#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <qlayout.h>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , app(new App())
    , timer(new QTimer(this))


{
    ARX* arx = new ARX();
    Sygnal* syg = new Sygnal();
    PID* pid = new PID(0.5, 5.0, 0.2);

    app->setSyg(syg);
    app->setARX(arx);
    app->setPID(pid);
    app->syg->setA(1);
    app->syg->sett(10);
    app->syg->setp(0.5);
    ui->setupUi(this);
    app->sygnal=0;
    app->a="-0.4";
    app->b="0,6";






    ui->comboBox->addItem("Jednostkowy", QVariant(0));
    ui->comboBox->addItem("Sinusoidalny", QVariant(1));
    ui->comboBox->addItem("Prostokontny", QVariant(2));




    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
    app->setk(1);
    app->a =   "-0.4";
    app->b =   "0.6";
    /*
     * QString input = ui->INPUTA->toPlainText();
    if (input.isEmpty()) {
        app->a="0";
        return;
    }

    // Wyświetl komunikat o sukcesie
    //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
    input = ui->INPUTB->toPlainText();
    if (input.isEmpty()) {
        app->b="0";
        return;
    }

    app->b = input.toStdString();

    //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
    */
    timer->start(app->getinterwal());
    // Tworzenie wykresu dla chart1
    chart1 = new QChart();
    chart1->setBackgroundVisible(true);

    // Up
    seriesUp = new QLineSeries();
    seriesUp->setName("Up");
    chart1->addSeries(seriesUp);

    // Ui
    seriesUi = new QLineSeries();
    seriesUi->setName("Ui");
    QPen peni(Qt::yellow);
    peni.setWidth(2);
    seriesUi->setPen(peni);
    chart1->addSeries(seriesUi);

    // Ud
    seriesUd = new QLineSeries();
    seriesUd->setName("Ud");
    QPen pend(Qt::green);
    pend.setWidth(2);
    seriesUd->setPen(pend);
    chart1->addSeries(seriesUd);

    // Uchyb
    seriesUchyb = new QLineSeries();
     seriesUchyb->setName("Uchyb");
    QPen pende(Qt::black);
    pende.setWidth(2);
     seriesUchyb->setPen(pende);
    chart1->addSeries( seriesUchyb);

    // Oś X
    axisX1 = new QValueAxis();
    axisX1->setRange(1, 100);
    axisX1->setTitleText("Czas");
    chart1->addAxis(axisX1, Qt::AlignBottom);

    // Oś Y
    axisY1 = new QValueAxis();
    axisY1->setRange(-0.1, 0.1);
    axisY1->setTitleText("Wartość");
    chart1->addAxis(axisY1, Qt::AlignLeft);



    // Podłączenie serii do osi
    seriesUp->attachAxis(axisX1);
    seriesUp->attachAxis(axisY1);

    seriesUi->attachAxis(axisX1);
    seriesUi->attachAxis(axisY1);

    seriesUd->attachAxis(axisX1);
    seriesUd->attachAxis(axisY1);

    seriesUchyb->attachAxis(axisX1);
    seriesUchyb->attachAxis(axisY1);

    // Tworzenie widoku wykresu
    chartView1 = new QChartView(chart1, this);
    chartView1->setRenderHint(QPainter::Antialiasing);

    // Dodanie wykresu do kontenera w interfejsie użytkownika
    ui->chartContainer_2->setLayout(new QVBoxLayout());
    ui->chartContainer_2->layout()->addWidget(chartView1);
    //wykres
    chart = new QChart();


    chart->setBackgroundVisible(true);








    //Y
    seriesY = new QLineSeries();
    seriesY->setName("Y");
    chart->addSeries(seriesY);



    //U
    seriesU = new QLineSeries();
    seriesU->setName("U");
    QPen pen(Qt::yellow);
    pen.setWidth(2);
    seriesU->setPen(pen);
    chart->addSeries(seriesU);
    //zalucenie


    //
    seriesW = new QLineSeries();
    seriesW->setName("Wejscie");
    QPen pen3(Qt::red);
    pen3.setWidth(2);
    seriesW->setPen(pen3);
    chart->addSeries(seriesW);

    //os X
    axisX = new QValueAxis();
    axisX->setRange(1, 100);
    axisX->setTitleText("Czas");
    chart->addAxis(axisX, Qt::AlignBottom);
    seriesY->attachAxis(axisX);
    seriesU->attachAxis(axisX);
    seriesW->attachAxis(axisX);


    //os y
    axisY = new QValueAxis();
    axisY->setRange(-0.1, 0.1);
    axisY->setTitleText("Wartość");
    chart->addAxis(axisY, Qt::AlignLeft);
    seriesY->attachAxis(axisY);
    seriesU->attachAxis(axisY);

    seriesW->attachAxis(axisY);
    //tworzenie widoku wykrersu
    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    //dodanie wykresu do widoku cartcontiner
    ui->chartContainer->setLayout(new QVBoxLayout());
    ui->chartContainer->layout()->addWidget(chartView);

    //conect(wysyjacys sygnal,sygnal emitowany pzez,odbiorca,lambda [this]
    //ma dstep do danych w mainwindow
    connect(timer, &QTimer::timeout, this, [this]() {
        app->symulacjaStep();
        updateChart();
    });
}

MainWindow::~MainWindow()
{   delete app;
    delete ui;
}



void MainWindow::setUIFromApp()
{
    // Ustawienia SpinBoxów i ComboBoxa
    ui->SPINBOXAMPLITUDA->setValue(app->syg->axx());
    ui->SPINBOXOKRES->setValue(app->syg->getT());
    ui->SPINBOXWYPELNIENIE->setValue(app->syg->getP());

    ui->SPINBOXKD->setValue(app->Pid->getWzmocnienieRowniczkujace());
    ui->SPINBOXKI->setValue(app->Pid->getWzmocnienieCalkujace());
    ui->SPINBOXKP->setValue(app->Pid->getWzmocnienieProporcjonalne());

    ui->SPINBOXINTERWAL->setValue(app->getinterwal());
    ui->comboBox->setCurrentIndex(app->sygnal);

    // Jeśli kiedyś będziesz używać INPUTA i INPUTB
    // ui->INPUTA->setPlainText(QString::fromStdString(app->a));
    // ui->INPUTB->setPlainText(QString::fromStdString(app->b));

    // Analogicznie, jeśli masz jeszcze inne elementy, ustaw je tutaj.
}



void MainWindow::on_START_clicked() {
     if (app) {
    app->startSymulacja();
         qDebug()<<app->getinterwal();
    timer->start(app->getinterwal());      // timer co 100 ms
    //qDebug() << "Symulacja uruchomiona.";
    ui->START->setEnabled(false);
    ui->STOP->setEnabled(true);


    //ui->SAVE->setDisabled(true);
     }
    else {
         qDebug() << "Błąd: Obiekt `App` nie został zainicjalizowany.";
    }
}

void MainWindow::on_STOP_clicked() {
    if (app) {
    app->stopSymulacja();  // Zatrzymaj symulację
    timer->stop();         // Zatrzymaj timer
   // qDebug() << "Symulacja zatrzymana.";
    ui->STOP->setEnabled(false);
    ui->START->setEnabled(true);

   // ui->SAVE->setDisabled(false);

    }
    else {
         qDebug() << "Błąd: `app->data` to nullptr";
     }
}

void MainWindow::on_pushButton_clicked()
{
    if (app) { //app?
        app->zapisBazydanychdopliku();


    }
    else {
        qDebug() << "`app->data` to nullptr";
    }
}

void MainWindow::on_ODCZYT_clicked()
{
    if (app) { //app?
        clearAllSeries();


        //restart X Y
       resetAxes();
        for (auto& elem : app->data) {
            delete elem; // usun dynamicznie  obiekty
        }
        app->data.clear();

        //
        BuforDanych::licznikInstancji=-1;
        app->odczytajDaneZPliku();

        if (app->data.empty()) {
            qDebug() << "`app->data` to nullptr";
            return;
        }
        for (auto& ostatniElement : app->data) {
            if (ostatniElement) {
                seriesY->append(ostatniElement->getI(), ostatniElement->getY());
                seriesU->append(ostatniElement->getI(), ostatniElement->getU());

                seriesW->append(ostatniElement->getI(), ostatniElement->getW());
                seriesUp->append(ostatniElement->getI(), ostatniElement->getUp());
                seriesUi->append(ostatniElement->getI(), ostatniElement->getUi());
                seriesUd->append(ostatniElement->getI(), ostatniElement->getUd());
                seriesUchyb->append(ostatniElement->getI(), ostatniElement->getUchyb());
                //Rozszerzenie zakresu osi X
                if (ostatniElement->getI() > axisX->max()*0.80 ) {
                    axisX->setRange(axisX->min() + 1, axisX->max() + 1);
                    axisX1->setRange(axisX1->min() + 1, axisX1->max() + 1);
                }
                if (ostatniElement->getY() < axisY->min()*0.8 ||
                    ostatniElement->getW()<  axisY->min()*0.8 ||
                    ostatniElement->getU()<axisY->min()*0.8
                    ) {
                    axisY->setRange(axisY->min() *1.8, axisY->max() );
                }
                if (ostatniElement->getY() > axisY->max()*0.8 ||
                    ostatniElement->getW()>  axisY->max()*0.8 ||
                    ostatniElement->getU()> axisY->max()*0.8) {
                    axisY->setRange(axisY->min(), axisY->max()*1.8 );
                }
                if (ostatniElement->getUp() < axisY1->min()*0.8 ||
                    ostatniElement->getUchyb()< axisY1->min()*0.8 ||
                    ostatniElement->getUi()<axisY1->min()*0.8 ||
                    ostatniElement->getUd()<axisY1->min()*0.8) {
                    axisY1->setRange(axisY1->min()*1.8, axisY1->max() );
                }
                if (ostatniElement->getUp() > axisY1->max()*0.8 ||
                    ostatniElement->getUchyb()> axisY1->max()*0.8 ||
                    ostatniElement->getUi()>axisY1->max()*0.8 ||
                    ostatniElement->getUd()>axisY1->max()*0.8
                    ) {
                    axisY1->setRange(axisY1->min(), axisY1->max()*1.8 );
                }

            } else {
                qDebug() <<"`app->data` to nullptr";
            }
        }





        //app->wypiszDane();
        setUIFromApp();

        app->setinterwal(ui->SPINBOXINTERWAL->value());
        app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
        app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
        app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
        app->sygnal=ui->comboBox->currentData().toInt();
        app->syg->sett(ui->SPINBOXOKRES->value());
        app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
        app->syg->setA(ui->SPINBOXAMPLITUDA->value());
        /*
         *
         *
        ui->INPUTA->setText(QString::fromStdString(app->a));
        ui->INPUTB->setText(QString::fromStdString(app->b));
 ui->SPINBOXK->setValue(app->getk());



        app->setk(ui->SPINBOXK->value());
        //
    QString input = ui->INPUTA->toPlainText();
        if (input.isEmpty()) {
            app->a="0";
            return;
        }
        app->a = input.toStdString();
        // Wyświetl komunikat o sukcesie
        //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
        input = ui->INPUTB->toPlainText();
        if (input.isEmpty()) {
            app->b="0";
            return;
        }

        app->b = input.toStdString();
        */ //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
        timer->start(app->getinterwal());

    }
    else {
        qDebug() << "Blad: App niezainicjalizwane";
    }
}
void MainWindow::updateChart()
{
    if (app->data.empty()) {
        qDebug() << "Brak danych w `app->data`";
        return;
    }

    BuforDanych* ostatniElement = app->data.back();
    if (ostatniElement) {
        seriesY->append(ostatniElement->getI(), ostatniElement->getY());
        seriesU->append(ostatniElement->getI(), ostatniElement->getU());

        seriesW->append(ostatniElement->getI(), ostatniElement->getW());
        seriesUp->append(ostatniElement->getI(),app->Pid->Up);
        seriesUi->append(ostatniElement->getI(),app->Pid->Ui);
        seriesUd->append(ostatniElement->getI(),app->Pid->Ud);
        seriesUchyb->append(ostatniElement->getI(), ostatniElement->getUchyb());

        //zwiekszanie wykresy
        if (ostatniElement->getI() > axisX->max()*0.80 ) {
            axisX->setRange(axisX->min() + 1, axisX->max() + 1);
            axisX1->setRange(axisX1->min() + 1, axisX1->max() + 1);
        }
        if (ostatniElement->getY() < axisY->min()*0.8 ||
            ostatniElement->getW()<  axisY->min()*0.8 ||
            ostatniElement->getU()<axisY->min()*0.8
            ) {
            axisY->setRange(axisY->min() *1.8, axisY->max() );
        }
        if (ostatniElement->getY() > axisY->max()*0.8 ||
            ostatniElement->getW()>  axisY->max()*0.8 ||
            ostatniElement->getU()> axisY->max()*0.8) {
            axisY->setRange(axisY->min(), axisY->max()*1.8 );
        }
        if (ostatniElement->getUp() < axisY1->min()*0.8 ||
            ostatniElement->getUchyb()< axisY1->min()*0.8 ||
            ostatniElement->getUi()<axisY1->min()*0.8 ||
            ostatniElement->getUd()<axisY1->min()*0.8) {
            axisY1->setRange(axisY1->min() *1.8, axisY1->max() );
        }
        if (ostatniElement->getUp() > axisY1->max()*0.8 ||
            ostatniElement->getUchyb()> axisY1->max()*0.8 ||
            ostatniElement->getUi()>axisY1->max()*0.8 ||
            ostatniElement->getUd()>axisY1->max()*0.8
            ) {
            axisY1->setRange(axisY1->min(), axisY1->max()*1.8 );
        }



    } else {
        qDebug() << "`app->data` to nullptr";
    }
}

void MainWindow::on_RESET_clicked()
{
    clearAllSeries();

    resetAxes();
    //czyszczenie bazy
    for (auto& elem : app->data) {
        delete elem; // usun dynamicznie  obiekty
    }
    app->data.clear();

    BuforDanych::licznikInstancji=-1;
    app->Pid->resetPamieci();




    ui->SPINBOXAMPLITUDA->setValue(1);
    ui->SPINBOXOKRES->setValue(10);
    ui->SPINBOXWYPELNIENIE->setValue(0.5);
    //
    ui->SPINBOXKD->setValue(0.5);
    ui->SPINBOXKI->setValue(5);
    ui->SPINBOXKP->setValue(0.2);
    //
    ui->SPINBOXINTERWAL->setValue(100);

    ui->comboBox->setCurrentIndex(0);

    //
    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());

    app->Pid->poprzedniUchyb=0;
    app->Pid->Ti=0;
    app->Pid->uchyb=0;
    app->oldY=0;

    //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
    timer->start(app->getinterwal());
}
void MainWindow::on_POMOC_clicked()
{//wzmocnienieProporcjonalne(kp), wzmocnienieCalkujace(ki), wzmocnienieRowniczkujace(kd),
    QString instrukcja = "Instrukcja obsługi aplikacji:\n\n"
                         "1. Wprowadź wartości do pól 'USTAW A' i 'USTAW B' w odpowiednim formacie (np. 0.4;-0.2).\n"
                         "2. Ustaw współczynniki Kp, Ki, Kd za pomocą pól na dole interfejsu.\n"
                         "3. Kliknij przycisk 'START', aby rozpocząć symulację.\n"
                         "4. Użyj przycisku 'RESET', aby zresetować ustawienia. Reset usuwa niezapisane dane\n"
                         "5. Możesz zapisać dane za pomocą przycisku 'ZAPISZ' lub wczytać dane z pliku za pomocą 'ODCZYTAJ Z PLIKU'.\n\n"
                         "Rozwiniecie Skrótów\n\n"
                         "A - Wspolczynik wielomianu A \n"
                         "A - Wspolczynik wielomianu B \n"
                         "Kp - Wzmocnienie Proporcjonalne \n"
                         "Ti - Wzmocnienie Calkujace\n"
                         "Td - Wzmocnienie Rowniczkujace\n";
     QMessageBox::information(this, "POMOC",instrukcja);






}

void MainWindow::on_BTNUSTAWALL_clicked()
{

    //
    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
   /* QString input = ui->INPUTA->toPlainText();
    if (input.isEmpty()) {
       app->a="0";
        return;
    }
    app->a = input.toStdString();
    // Wyświetl komunikat o sukcesie
    //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
     input = ui->INPUTB->toPlainText();
    if (input.isEmpty()) {
         app->b="0";
        return;
    }

    app->b = input.toStdString();
    //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
 */
  timer->start(app->getinterwal());
}


void MainWindow::on_BTNZAPISZCONF_clicked()
{
    app->zapisconf();
}


void MainWindow::on_pushButton_2_clicked()
{

    setUIFromApp();



    //app->setk(ui->SPINBOXK->value());
    //
    app->setinterwal(ui->SPINBOXINTERWAL->value());
    app->Pid->ustawWzmocnienieCalkujace(ui->SPINBOXKI->value());
    app->Pid->ustawWzmocnienieProporcjonalne(ui->SPINBOXKP->value());
    app->Pid->ustawWzmocnienieRowniczkujace(ui->SPINBOXKD->value());
    app->sygnal=ui->comboBox->currentData().toInt();
    app->syg->sett(ui->SPINBOXOKRES->value());
    app->syg->setp(ui->SPINBOXWYPELNIENIE->value());
    app->syg->setA(ui->SPINBOXAMPLITUDA->value());
    /*QString input = ui->INPUTA->toPlainText();
    if (input.isEmpty()) {
        app->a="0";
        return;
    }
    app->a = input.toStdString();
    // Wyświetl komunikat o sukcesie
    //QMessageBox::information(this, "Sukces", "Wartość A została ustawiona jako: " + input);
    input = ui->INPUTB->toPlainText();
    if (input.isEmpty()) {
        app->b="0";
        return;
    }

    app->b = input.toStdString();
    */ //QMessageBox::information(this, "Sukces", "Wartość B została ustawiona jako: " + input);
    timer->start(app->getinterwal());


}


void MainWindow::on_pushButton_3_clicked()
{
    arx_window = new ARXwindow(nullptr,app);
    arx_window->show();


}
void MainWindow::clearAllSeries()
{
    if (seriesY)  seriesY->clear();
    if (seriesU)  seriesU->clear();
    if (seriesW)  seriesW->clear();
    if (seriesUp) seriesUp->clear();
    if (seriesUd) seriesUd->clear();
    if (seriesUi) seriesUi->clear();
    if (seriesUchyb) seriesUchyb->clear();
}
void MainWindow::resetAxes()
{
    if (axisX)  axisX->setRange(0, 100);
    if (axisY)  axisY->setRange(-0.1, 0.1);
    if (axisX1) axisX1->setRange(0, 100);
    if (axisY1) axisY1->setRange(-0.1, 0.1);
}
