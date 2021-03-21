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
    ui->textReceive->append("new Client connected");
}
