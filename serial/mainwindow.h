#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"QSerialPort"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort *SerialPort;
    QTimer *fTimer;    //定时器

private slots:
    void on_closeport_clicked();

    void on_openport_clicked();

    void on_senddata_clicked();

    void serialPortReadyRead_Slot();

    void receivetime_Slot();

    void on_clr_clicked();

    void on_outputform_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
