#include "tcpserver.h"
#include "ui_tcpserver.h"

tcpserver::tcpserver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcpserver)
{
    ui->setupUi(this);

    m_server = new QTcpServer(this);

    connect(ui->ButtonConnect, SIGNAL(released()), this, SLOT(buttonListenPressed()));
}

tcpserver::~tcpserver()
{
    delete ui;
}

void tcpserver::buttonListenPressed(){
    if(m_server->listen(QHostAddress::Any,ui->linePort->text().toUInt())){
        connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnectionHandler()));
    }else{
        ui->textReceive->append(QString("error: %1").arg(m_server->errorString()));
    }
}

void tcpserver::newConnectionHandler(){
    while (m_server->hasPendingConnections()){
        QTcpSocket *new_client = m_server->nextPendingConnection();
        tcpClients.insert(new_client);
        connect(new_client, SIGNAL(readyRead()),this, SLOT(receiveData()));
        connect(new_client, SIGNAL(disconnected()), this, SLOT(disconnectHandler()));
    }
           //appendToSocketList(m_server->nextPendingConnection());
    ui->textReceive->append("new Client connected");
}

void tcpserver::receiveData(){
    QTcpSocket *new_socket = reinterpret_cast<QTcpSocket *>(sender());
    QByteArray msg = new_socket->readAll();
    ui->textReceive->append(QString(msg));
}

void tcpserver::disconnectHandler(){
    QTcpSocket *new_socket = reinterpret_cast<QTcpSocket *>(sender());
    QSet<QTcpSocket *>::Iterator it = tcpClients.find(new_socket);
    if(it != tcpClients.end()){
        ui->textReceive->append(QString("client disconnected"));
        tcpClients.remove(*it);
    }
    new_socket->deleteLater();
}
