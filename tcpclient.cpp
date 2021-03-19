#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QHostAddress>

TCPClient::TCPClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPClient)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);
    tcpTimer = new QTimer(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(client_connected()));
    connect(ui->ButtonConnect, SIGNAL(released()), this, SLOT(buttonConnectPressed()));
    connect(ui->ButtonSend, SIGNAL(released()), this, SLOT(buttonSendpressed()));
    connect(ui->ButtonClear, SIGNAL(released()), this, SLOT(buttonClearpressed()));
    connect(ui->ButtonStart, SIGNAL(released()), this, SLOT(buttonStartpressed()));
    connect(tcpTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));

    ui->lineIP->setText("192.168.1.46");
    ui->linePort->setText("7");

    connect_status = false;
}

TCPClient::~TCPClient()
{
    delete ui;
}

void TCPClient::onReadyRead(){
    QByteArray datas = tcpSocket->readAll();
    QString msg(datas);
    ui->textReceive->append(msg);
}
void TCPClient::client_connected(){
    QString msg = "Connected to: " + ui->lineIP->text() + ":" + ui->linePort->text();
    ui->textReceive->append(msg);
    ui->ButtonConnect->setText("Disconnect");
    connect_status = true;
}
void TCPClient::buttonConnectPressed(){
    if(!connect_status){
        QString Ip_str = ui->lineIP->text();
        QString Port_str = ui->linePort->text();
        tcpSocket->connectToHost(QHostAddress(Ip_str), Port_str.toInt());
    }else{
        tcpSocket->disconnectFromHost();
        if (tcpSocket->state() == QAbstractSocket::UnconnectedState
            || tcpSocket->waitForDisconnected(1000)) {
                ui->textReceive->append("Disconnected");
                ui->ButtonConnect->setText("connect");
                connect_status = false;
        }
    }
}
void TCPClient::buttonSendpressed(){
    QString textStr = ui->lineSend->text();
    QByteArray bytes = textStr.toUtf8();
    tcpSocket->write(bytes);
}
void TCPClient::buttonClearpressed(){

}

void TCPClient::buttonStartpressed(){
    if(isAutoSend){
        tcpTimer->stop();
        isAutoSend = false;
        ui->ButtonStart->setText("START");
        ui->lineEditSetTxt->setEnabled(true);
        ui->lineEditTimeout->setEnabled(true);
    }else{
        QString timeStr = ui->lineEditTimeout->text();
        int time = timeStr.toInt();
        if(time < 0)
            return;
        tcpTimer->setInterval(time);
        tcpTimer->start();
        isAutoSend = true;
        ui->ButtonStart->setText("STOP");
        ui->lineEditSetTxt->setEnabled(false);
        ui->lineEditTimeout->setEnabled(false);
    }
}

void TCPClient::timerTimeout(){
    QString textStr = ui->lineEditSetTxt->text();
    QByteArray bytes = textStr.toUtf8();
    /*
    if(ui->checkBox_CR->isChecked())
        bytes.append((char)0x0D); //add \r

    if(ui->checkBox_LF->isChecked())
        bytes.append((char)0x0A); //add \n
    */
    tcpSocket->write(bytes);
}
