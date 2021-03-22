#include "tcpserver.h"
#include "ui_tcpserver.h"

tcpserver::tcpserver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcpserver)
{
    ui->setupUi(this);

    m_server = new QTcpServer(this);

    connect(ui->ButtonConnect, SIGNAL(released()), this, SLOT(buttonListenPressed()));
    connect(ui->ButtonSend, SIGNAL(released()), this, SLOT(buttonSendPressed()));

    connected_clients = 0;
    is_listening = false;
    setWidgetEnables(false);
}

tcpserver::~tcpserver()
{
    foreach(QTcpSocket *socket, tcpClients){
        socket->close();
        socket->deleteLater();
    }
    m_server->close();
    m_server->deleteLater();
    delete ui;
}

void tcpserver::buttonListenPressed(){
    if(!is_listening){
        if(m_server->listen(QHostAddress::Any,ui->linePort->text().toUInt())){
            connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionHandler()));
            ui->textReceive->append(QString("Server is listenning at port: %1").arg(ui->linePort->text()));
            ui->ButtonConnect->setText("Stop");
            is_listening = true;
            setWidgetEnables(true);
        }else{
            ui->textReceive->append(QString("error: %1").arg(m_server->errorString()));
        }
    }else{
        is_listening = false;
        setWidgetEnables(false);
        ui->ButtonConnect->setText("Listen");
        //close all clients connections
        foreach(QTcpSocket *socket, tcpClients){
            socket->close();
            socket->deleteLater();
        }
        //close server
        m_server->close();
        ui->textReceive->append("Server stopped");
    }
}

void tcpserver::newConnectionHandler(){
    while (m_server->hasPendingConnections()){
        QTcpSocket *new_client = m_server->nextPendingConnection();
        tcpClients.insert(new_client);
        connect(new_client, SIGNAL(readyRead()),this, SLOT(receiveData()));
        connect(new_client, SIGNAL(disconnected()), this, SLOT(disconnectHandler()));
        connected_clients++;
        ui->labelClientsNum->setText(tr("%1").arg(connected_clients));
    }
           //appendToSocketList(m_server->nextPendingConnection());
    ui->textReceive->append("new Client connected");
}

void tcpserver::receiveData(){
    QTcpSocket *new_socket = reinterpret_cast<QTcpSocket *>(sender());
    QByteArray msg = new_socket->readAll();
    ui->textReceive->append(QString(msg));
    if(ui->checkBox_ECHO->isChecked()){
        new_socket->write(msg);
        ui->textSend->append(QString(msg));
    }
}

void tcpserver::disconnectHandler(){
    QTcpSocket *new_socket = reinterpret_cast<QTcpSocket *>(sender());
    QSet<QTcpSocket *>::Iterator it = tcpClients.find(new_socket);
    if(it != tcpClients.end()){
        ui->textReceive->append(QString("client disconnected"));
        tcpClients.remove(*it);
        connected_clients--;
        ui->labelClientsNum->setText(tr("%1").arg(connected_clients));
    }
    new_socket->deleteLater();
}

void tcpserver::buttonSendPressed(){
    QString msg = ui->lineSend->text();
    QByteArray bytes = msg.toUtf8();
    if(ui->checkBox_CR->isChecked())
        bytes.append((char)0x0D); //add \r

    if(ui->checkBox_LF->isChecked())
        bytes.append((char)0x0A); //add \n

    //send message to all clients
    foreach(QTcpSocket *socket, tcpClients){
        socket->write(bytes);
    }
}

void tcpserver::setWidgetEnables(bool state){
    ui->ButtonClear->setEnabled(state);
    ui->ButtonSend->setEnabled(state);
    ui->ButtonStart->setEnabled(state);
    ui->checkBox_CR->setEnabled(state);
    ui->checkBox_LF->setEnabled(state);
    ui->textReceive->setEnabled(state);
    ui->lineEditSetTxt->setEnabled(state);
    ui->lineEditTimeout->setEnabled(state);
    ui->lineSend->setEnabled(state);
    ui->textSend->setEnabled(state);
}
