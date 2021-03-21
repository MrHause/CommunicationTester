#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSet>

namespace Ui {
class tcpserver;
}

class tcpserver : public QWidget
{
    Q_OBJECT

public:
    explicit tcpserver(QWidget *parent = nullptr);
    ~tcpserver();

private slots:
    void buttonListenPressed();
    void newConnectionHandler();

private:
    Ui::tcpserver *ui;
    QSet<QTcpSocket *> tcpClients;
    QTcpServer *m_server;
};

#endif // TCPSERVER_H
