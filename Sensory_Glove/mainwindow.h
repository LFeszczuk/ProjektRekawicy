#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QtCore/QTimer>
#include <QElapsedTimer>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
QString Data;
private slots:
    void on_szukaj_clicked();

    void on_polacz_clicked();

    void on_roalacz_clicked();

    void readFromPort();

    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;

    QSerialPort *device;

    QByteArray charBuffer;

    QTimer dataTimer;

    double Value[16];

};
#endif // MAINWINDOW_H
