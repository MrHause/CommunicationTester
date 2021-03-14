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
    connect(ui->ButtonStart, SIGNAL(released()), this, SLOT(buttonStartPressed()));

    //serial
    m_serial = new QSerialPort();
    connect(m_serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));

    //timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));

    fillParameters();
}

Serial::~Serial()
{
    delete ui;
}

void Serial::buttonSetPressed(){
    if(portIsOpened){
        ui->textEditReceive->setStyleSheet("color: orange;  background-color: white");
        ui->textEditReceive->append("Port Closed");
        ui->textEditReceive->setStyleSheet("color: black;  background-color: white");
        ui->ButtonSet->setText("Open");
        ui->ListBaudRate->setEnabled(true);
        ui->ListDataBits->setEnabled(true);
        ui->ListFlowCtrl->setEnabled(true);
        ui->ListParity->setEnabled(true);
        ui->ListSerialPorts->setEnabled(true);
        ui->ListSropBits->setEnabled(true);
        ui->textEditReceive->setEnabled(false);
        ui->ButtonSend->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->lineEditSetTxt->setEnabled(false);
        ui->lineEditTimeout->setEnabled(false);
        ui->ButtonSend->setEnabled(false);
        portIsOpened = false;
        m_serial->close();
    }else{
        m_serial->setBaudRate(ui->ListBaudRate->currentData().toInt());
        m_serial->setDataBits(static_cast<QSerialPort::DataBits>(ui->ListDataBits->itemData(ui->ListDataBits->currentIndex()).toInt()));
        m_serial->setParity(static_cast<QSerialPort::Parity>(ui->ListParity->itemData(ui->ListParity->currentIndex()).toInt()));
        m_serial->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->ListFlowCtrl->itemData(ui->ListFlowCtrl->currentIndex()).toInt()));
        m_serial->setStopBits(static_cast<QSerialPort::StopBits>(ui->ListSropBits->itemData(ui->ListSropBits->currentIndex()).toInt()));
        m_serial->setPortName(ui->ListSerialPorts->currentText());

        if( m_serial->open(QIODevice::ReadWrite) ){
            qDebug() << "Connected to "<< ui->ListSerialPorts->currentText();
            ui->textEditReceive->setStyleSheet("color: orange;  background-color: white");
            ui->textEditReceive->append(ui->ListSerialPorts->currentText());
            ui->textEditReceive->setStyleSheet("color: black;  background-color: white");
            portIsOpened = true;
            ui->ButtonSet->setText("Close");
            ui->ListBaudRate->setEnabled(false);
            ui->ListDataBits->setEnabled(false);
            ui->ListFlowCtrl->setEnabled(false);
            ui->ListParity->setEnabled(false);
            ui->ListSerialPorts->setEnabled(false);
            ui->ListSropBits->setEnabled(false);
            ui->textEditReceive->setEnabled(true);
            ui->ButtonSend->setEnabled(true);
            ui->lineEdit->setEnabled(true);
            ui->lineEditSetTxt->setEnabled(true);
            ui->lineEditTimeout->setEnabled(true);
            ui->ButtonStart->setEnabled(true);
        }else{
            qDebug() << "Error during connecting";
        }
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
    if(ui->checkBox_CR->isChecked())
        bytes.append((char)0x0D); //add \r

    if(ui->checkBox_LF->isChecked())
        bytes.append((char)0x0A); //add \n

    m_serial->write(bytes);
    //m_serial->setDataBits(ui->ListDataBits->);
}

void Serial::fillParameters(){
    //-----------serial parameters--------------
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

    //------------widget setups------------------------
    ui->textEditReceive->setReadOnly(true);
    portIsOpened = false;
    isAutoSend = false;
    ui->textEditReceive->setEnabled(false);
    ui->ButtonSend->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->lineEditSetTxt->setEnabled(false);
    ui->lineEditTimeout->setEnabled(false);
    ui->ButtonStart->setEnabled(false);
}

void Serial::readSerialData(){
    QByteArray datas = m_serial->readAll();
    QString msg(datas);
    ui->textEditReceive->append(msg);
    qDebug() << datas;
}

void Serial::buttonStartPressed(){
    if(isAutoSend){
        timer->stop();
        isAutoSend = false;
        ui->ButtonStart->setText("START");
        ui->lineEditSetTxt->setEnabled(true);
        ui->lineEditTimeout->setEnabled(true);
    }else{
        QString timeStr = ui->lineEditTimeout->text();
        int time = timeStr.toInt();
        if(time < 0)
            return;
        timer->setInterval(time);
        timer->start();
        isAutoSend = true;
        ui->ButtonStart->setText("STOP");
        ui->lineEditSetTxt->setEnabled(false);
        ui->lineEditTimeout->setEnabled(false);
    }
}

void Serial::timerTimeout(){
    QString textStr = ui->lineEditSetTxt->text();
    QByteArray bytes = textStr.toUtf8();
    if(ui->checkBox_CR->isChecked())
        bytes.append((char)0x0D); //add \r

    if(ui->checkBox_LF->isChecked())
        bytes.append((char)0x0A); //add \n

    m_serial->write(bytes);
}



