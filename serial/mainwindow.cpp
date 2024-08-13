#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QRegularExpressionValidator>

//全局变量

bool gsendhex = false;
bool greceivehex = false;
int  galreadyreceive = 0;
bool gshowtime = true;
bool gshowRec = true;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SerialPort = new QSerialPort(this);
    fTimer=new QTimer(this);  //新建定时器类

    //手动关联槽函数
 //   connect(SerialPort, SIGNAL(readyRead()), this, SLOT(serialPortReadyRead_Slot()));
 //   connect(fTimer,SIGNAL(timeout()),this,SLOT(receivetime_Slot()));
    connect(SerialPort, SIGNAL(readyRead()), this, SLOT(receivetime_Slot()));
    connect(fTimer,SIGNAL(timeout()),this,SLOT(serialPortReadyRead_Slot()));

     QStringList serialNamePort;
    foreach (const QSerialPortInfo &info , QSerialPortInfo::availablePorts())
    {
  //      serialNamePort << info.portName();
        serialNamePort << (info.portName()+"("+ info.description()+")");
 //       serialNamePort <<info.description() ;
    }
    ui->serialcb ->addItems(serialNamePort);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//接收槽函数
void MainWindow::receivetime_Slot()
{

//    fTimer->stop();         //启动前停止定时器，避免前面有未关闭的定时器
    fTimer->start (20) ;     //设置定时周期，单位：毫秒
    galreadyreceive =SerialPort->readBufferSize(); //目前缓冲中的数据
}


void MainWindow::serialPortReadyRead_Slot()
{
    QString buff;
    QByteArray data;

    QDateTime RecvTime ;
    RecvTime = QDateTime::currentDateTime(); //获取当前时间
    fTimer->stop();

    int receiveNow=SerialPort->readBufferSize();

    if(receiveNow ==galreadyreceive) //没有新数据
    {
        ;
    }
    else
    {
       fTimer->start (20) ;
        galreadyreceive =SerialPort->readBufferSize();
       return ;
    }

//    if(SerialPort->readBufferSize()>0)
 /*   {
        data =SerialPort->readAll();
        buff = QString(data.toHex());

        ui->reczone->appendPlainText(buff);
    }
 */

    if(gshowtime == true)
    {
        ui->reczone->appendPlainText("recv  "+RecvTime.toString());
    }
//    qDebug() << "Received data:" << data << "at" << time.toString(); //输出接收到的数据和时间信息
    if(greceivehex ==true)
    {

        data =SerialPort->readAll();
        buff = QString(data.toHex(' '));

        ui->reczone->appendPlainText(buff);
    }
    else
    {
        buff = QString(SerialPort->readAll());
        ui->reczone->appendPlainText(buff);
    }
}
void MainWindow::on_openport_clicked()
{
    QSerialPort::BaudRate baudRate;//定义波特率
    QSerialPort::DataBits dataBits;//定义数据位
    QSerialPort::StopBits stopBits;//定义停止位
    QSerialPort::Parity   patity;  //定义校验位

    int index;
    QStringList serialNamePort;
 //   QString serialNamePort;

    //波特率赋值
    if(ui->baudrate->currentText() == "1200")
    {
        baudRate = QSerialPort::Baud1200;
    }else if(ui->baudrate->currentText() == "2400")
    {
        baudRate = QSerialPort::Baud2400;
    }else if(ui->baudrate->currentText() == "4800")
    {
        baudRate = QSerialPort::Baud4800;
    }else if(ui->baudrate->currentText() == "9600")
    {
        baudRate = QSerialPort::Baud9600;
    }
    else if(ui->baudrate->currentText() == "57600")
    {
        baudRate = QSerialPort::Baud57600;
    }
    else if(ui->baudrate->currentText() == "115200")
    {
        baudRate = QSerialPort::Baud115200;
    }
    else
    {
        baudRate = QSerialPort::Baud9600;

    }
    //数据位赋值
    if(ui->datalength->currentText() == "5")
    {
        dataBits = QSerialPort::Data5;
    }else if(ui->datalength->currentText() == "6")
    {
        dataBits = QSerialPort::Data6;
    }else if(ui->datalength->currentText() == "7")
    {
        dataBits = QSerialPort::Data7;
    }else if(ui->datalength->currentText() == "8")
    {
        dataBits = QSerialPort::Data8;
    }
    else
    {
         dataBits = QSerialPort::Data8;
    }
    //停止位赋值
    if(ui->stopbit->currentText() == "1")
    {
        stopBits = QSerialPort::OneStop;
    }else if(ui->stopbit->currentText() == "1.5")
    {
        stopBits = QSerialPort::OneAndHalfStop;
    }else if(ui->stopbit->currentText() == "2")
    {
        stopBits = QSerialPort::TwoStop;
    }
    else
    {
        stopBits = QSerialPort::OneStop;
    }
    //校验位赋值
    if(ui->checkmethod ->currentText() == "None")
    {
        patity = QSerialPort::NoParity;
    }
    else if(ui->checkmethod->currentText() == "Even")
    {
        patity = QSerialPort::EvenParity;
    }else if(ui->checkmethod->currentText() == "Odd")
    {
        patity = QSerialPort::OddParity;
    }
    else
    {
       patity = QSerialPort::NoParity;
    }

    serialNamePort<<ui->serialcb->currentText();
    index = serialNamePort[0].indexOf('(');
 //   serialNamePort << serialNamePort.mid(0,index).join("");
    SerialPort->setPortName(serialNamePort[0].mid(0,index));
  //  SerialPort->setPortName(ui->serialcb->currentText()  );   //设置串口号 serialNamePort
    SerialPort->setBaudRate(baudRate);                //设置波特率
    SerialPort->setDataBits(dataBits);                //设置数据位
    SerialPort->setStopBits(stopBits);                //设置停止位
    SerialPort->setParity(patity);                   //设置校验位

    if(SerialPort->open(QIODevice::ReadWrite) == true)
    {//   需要
        QMessageBox::information(this,"提示","成功");
    }
    else
    {
        QMessageBox::critical(this,"提示","打开串口失败");
    }
}


void MainWindow::on_closeport_clicked()
{
    SerialPort->close();
}

void MainWindow::on_senddata_clicked()
{
    QDateTime SendTime ;
    if(gshowtime == true)
    {
        SendTime = QDateTime::currentDateTime(); //获取当前时间
        ui->reczone->appendPlainText("send  "+SendTime.toString());

    }

    QString info = ui->sendzone->text();
    if(info.contains(" "))
    {
        info.replace(QString(" "),QString(""));
    }

    QByteArray sendbuf = QByteArray::fromHex(info.toLatin1());


    if(gsendhex ==true)
    {
        SerialPort->write(sendbuf);
 //     SerialPort->write(ui->sendzone->text().toLocal8Bit().data());
    }
    else
    {
        ui->reczone->appendPlainText(ui->sendzone->text().toLocal8Bit().data());
        SerialPort->write(ui->sendzone->text().toLocal8Bit().data());
    }


}





void MainWindow::on_clr_clicked()
{
    ui->reczone->clear();
}

void MainWindow::on_outputform_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        greceivehex = true;
    }
    else
    {
        greceivehex = false;
    }
}


void MainWindow::on_outputform_2_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        //定义一个正则表达式，用于匹配十六进制数
        QRegularExpression hexRegex("^[0-9A-Fa-f ]+$");
        QValidator *validator = new QRegularExpressionValidator(hexRegex, this);
        //      QRegularExpressionValidator * validator = new QRegularExpression (hexRegex); // 将匹配模式设为验证对象
        //      ui->sendzone->validator(hexRegex);
        //      SerialPort->write(ui->sendzone->text());
        ui->sendzone->setValidator(validator); //限制十六进制
        gsendhex = true;

    }
    else
    {

        ui->sendzone->setValidator(NULL); //无限制
        gsendhex = false;

    }
}


void MainWindow::on_IfShowRec_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {

        gshowtime = false;

    }
    else
    {

        gshowtime= true;

    }
}


void MainWindow::on_IfShowTime_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {

        gshowRec = false;

    }
    else
    {

        gshowRec = true;

    }
}

