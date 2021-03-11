#include "serial.h"
#include "ui_serial.h"
#include "QList"
#include "qiodevice.h"


Serial::Serial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
    connect(ui->ButtonSend, SIGNAL(released()), this, SLOT(buttonSendPressed()));
    connect(ui->ButtonSet, SIGNAL(released()), this, SLOT(buttonSetPressed()));

    m_serial = new QSerialPort();

    connect(m_serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
    fillParameters();
}

Serial::~Serial()
{
    delete ui;
}

void Serial::buttonSetPressed(){
    m_serial->setBaudRate(ui->ListBaudRate->currentData().toInt());
    m_serial->setDataBits(static_cast<QSerialPort::DataBits>(ui->ListDataBits->itemData(ui->ListDataBits->currentIndex()).toInt()));
    m_serial->setParity(static_cast<QSerialPort::Parity>(ui->ListParity->itemData(ui->ListParity->currentIndex()).toInt()));
    m_serial->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->ListFlowCtrl->itemData(ui->ListFlowCtrl->currentIndex()).toInt()));
    m_serial->setStopBits(static_cast<QSerialPort::StopBits>(ui->ListSropBits->itemData(ui->ListSropBits->currentIndex()).toInt()));
    m_serial->setPortName(ui->ListSerialPorts->currentText());

    if( m_serial->open(QIODevice::ReadWrite) ){
        qDebug() << "Connected to "<< ui->ListSerialPorts->currentText();
    }else{
        qDebug() << "Error during connecting";
    }
}

void Serial::SerialInit(){
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        ui->ListSerialPorts->addItem(serialPortInfo.portName());
     }
    ui->ListDataBits->setCurrentIndex(3); //set default 8 bits
}

void Serial::buttonSendPressed(){
    QString textStr = ui->lineEdit->text();
    QByteArray bytes = textStr.toUtf8();
    bytes.append((char)0x0A); //add CR
    m_serial->write(bytes);
    //m_serial->setDataBits(ui->ListDataBits->);
}

void Serial::fillParameters(){
    ui->ListBaudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->ListBaudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->ListBaudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->ListBaudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    //ui->ListBaudRate->addItem(tr("Custom"));

    ui->ListDataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->ListDataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->ListDataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->ListDataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->ListDataBits->setCurrentIndex(3);

    ui->ListParity->addItem(tr("None"), QSerialPort::NoParity);
    ui->ListParity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->ListParity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->ListParity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->ListParity->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->ListSropBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->ListSropBits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->ListSropBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->ListFlowCtrl->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->ListFlowCtrl->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->ListFlowCtrl->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void Serial::readSerialData(){
    QByteArray datas = m_serial->readAll();
    QString msg(datas);
    ui->textEditReceive->append(msg);
    qDebug() << datas;
}



