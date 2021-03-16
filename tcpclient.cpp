#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QHostAddress>

TCPClient::TCPClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPClient)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(client_connected()));
    connect(ui->ButtonConnect, SIGNAL(released()), this, SLOT(buttonConnectPressed()));
    connect(ui->ButtonSend, SIGNAL(released()), this, SLOT(buttonSendpressed()));
    connect(ui->ButtonClear, SIGNAL(released()), this, SLOT(buttonClearpressed()));

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
