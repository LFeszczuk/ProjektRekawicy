#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QtCore/QTimer>
#include <QElapsedTimer>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);

    QList<QCustomPlot *> widgets = this->findChildren<QCustomPlot *>();
    for(int i=0;i<widgets.size();i++)
    {
        widgets[i]->addGraph(); // red line
        widgets[i]->graph(0)->setPen(QPen(QColor(255, 110, 40)));
        qDebug()<<widgets[i];

    }
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    for(int i=0;i<widgets.size();i++){
        widgets[i]->xAxis->setTicker(timeTicker);
        widgets[i]->axisRect()->setupFullAxesBox();
        widgets[i]->yAxis->setRange(0, 1023);
    }
    // make left and bottom axes transfer their ranges to right and top axes:
    for(int i=0;i<widgets.size();i++){
        connect(widgets[i]->xAxis, SIGNAL(rangeChanged(QCPRange)), widgets[i]->xAxis2, SLOT(setRange(QCPRange)));
        connect(widgets[i]->yAxis, SIGNAL(rangeChanged(QCPRange)), widgets[i]->yAxis2, SLOT(setRange(QCPRange)));
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_szukaj_clicked()
{

    qDebug() << "Szukam urządzeń";
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++) {
        qDebug() << devices.at(i).portName() << devices.at(i).description();
        ui->comboBoxdevices->addItem(devices.at(i).portName());
    }
}

void MainWindow::on_polacz_clicked()
{
    if(ui->comboBoxdevices->count() == 0) {
        qDebug() <<"Nie wykryto żadnych urządzeń!";
        return;
    }

    QString portName = ui->comboBoxdevices->currentText().split("\t").first();
    this->device->setPortName(portName);
    qDebug() <<portName;
    // OTWÓRZ I SKONFIGURUJ PORT:
    if(device->open(QSerialPort::ReadWrite)) {
        this->device->setBaudRate(QSerialPort::Baud9600);
        this->device->setDataBits(QSerialPort::Data8);
        this->device->setParity(QSerialPort::NoParity);
        this->device->setStopBits(QSerialPort::OneStop);
        this->device->setFlowControl(QSerialPort::NoFlowControl);
        connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
        qDebug() <<"Otwarto port szeregowy.";
    } else {
        qDebug() <<"Otwarcie porty szeregowego się nie powiodło!";
    }
}

void MainWindow::on_roalacz_clicked()
{
    if(this->device->isOpen()) {
        this->device->close();
        qDebug() <<"Zamknięto połączenie.";
    } else {
        qDebug() <<"Port nie jest otwarty!";
        return;
    }
}

void MainWindow::readFromPort() {
    QList<QCustomPlot *> widgets = this->findChildren<QCustomPlot *>();
    while(this->device->canReadLine()) {

        static QTime time(QTime::currentTime());

        QByteArray data_in = this->device->readAll();

        double key = time.elapsed()/1000.0;
        static double lastPointKey = 0;

        charBuffer.append(data_in);
        //qDebug()<<charBuffer;
        if (data_in.contains("\n")) {
            QString myString(charBuffer);
            if(myString.startsWith("P")){
                Data=myString;

                QStringList List=Data.split('-');

                for(int i=1;i<List.size()-1;i++)
                {
                    Value[i-1]=List[i].toDouble();
                   // qDebug()<<Value[i-1];
                }
            }



            if (key-lastPointKey > 0.002) // at most add point every 2 ms
            {
                // add data to lines:
                for(int i=0;i<widgets.size();i++){
                    widgets[i]->graph(0)->addData(key, Value[i]);
                }
                lastPointKey = key;
            }
            // make key axis range scroll with the data (at a constant range size of 8):
            for(int i=0;i<widgets.size();i++){
                widgets[i]->xAxis->setRange(key, 8, Qt::AlignRight);
                widgets[i]->replot();
            }
            // calculate frames per second:
            static double lastFpsKey;
            static int frameCount;
            ++frameCount;
            if (key-lastFpsKey > 2) // average fps over 2 seconds
            {
                lastFpsKey = key;
                frameCount = 0;
            }


            charBuffer = "";
        }
    }
}

void MainWindow::realtimeDataSlot()
{


}

