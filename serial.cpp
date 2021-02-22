#include "serial.h"
#include "ui_serial.h"
#include "QList"


Serial::Serial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
    connect(ui->ButtonSend, SIGNAL(released()), this, SLOT(buttonSendPressed()));
    connect(ui->ButtonSet, SIGNAL(released()), this, SLOT(buttonSetPressed()));

    m_serial = new QSerialPort();
    fillParameters();
}

Serial::~Serial()
{
    delete ui;
}

void Serial::buttonSetPressed(){

}

void Serial::SerialInit(){
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        ui->ListSerialPorts->addItem(serialPortInfo.portName());
     }
    ui->ListDataBits->setCurrentIndex(3); //set default 8 bits
}

void Serial::buttonSendPressed(){
    m_serial->setBaudRate(ui->ListBaudRate->currentData().toInt());
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

void Serial::updateSettings(){
    /*
    m_currentSettings.name = m_ui->serialPortInfoListBox->currentText();

    if (m_ui->baudRateBox->currentIndex() == 4) {
        m_currentSettings.baudRate = m_ui->baudRateBox->currentText().toInt();
    } else {
        m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    m_ui->baudRateBox->itemData(m_ui->baudRateBox->currentIndex()).toInt());
    }
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                m_ui->dataBitsBox->itemData(m_ui->dataBitsBox->currentIndex()).toInt());
    m_currentSettings.stringDataBits = m_ui->dataBitsBox->currentText();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
                m_ui->parityBox->itemData(m_ui->parityBox->currentIndex()).toInt());
    m_currentSettings.stringParity = m_ui->parityBox->currentText();

    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                m_ui->stopBitsBox->itemData(m_ui->stopBitsBox->currentIndex()).toInt());
    m_currentSettings.stringStopBits = m_ui->stopBitsBox->currentText();

    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                m_ui->flowControlBox->itemData(m_ui->flowControlBox->currentIndex()).toInt());
    m_currentSettings.stringFlowControl = m_ui->flowControlBox->currentText();

    m_currentSettings.localEchoEnabled = m_ui->localEchoCheckBox->isChecked();
    */
}


