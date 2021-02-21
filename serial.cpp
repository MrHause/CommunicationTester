#include "serial.h"
#include "ui_serial.h"
#include "QList"
#include "QtSerialPort"
#include "QSerialPortInfo"


Serial::Serial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(buttonSetPressed()));
}

Serial::~Serial()
{
    delete ui;
}

void Serial::buttonSetPressed(){
    int a = 0;
    a = 10;
}

void Serial::SerialInit(){
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        ui->ListSerialPorts->addItem(serialPortInfo.portName());
     }
}
