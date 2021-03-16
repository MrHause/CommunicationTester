#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class TCPClient;
}

class TCPClient : public QWidget
{
    Q_OBJECT

public:
    explicit TCPClient(QWidget *parent = nullptr);
    ~TCPClient();

private slots:
    void onReadyRead();
    void client_connected();
    void buttonConnectPressed();
    void buttonSendpressed();
    void buttonClearpressed();

private:
    Ui::TCPClient *ui;
    QTcpSocket *tcpSocket;
    bool connect_status;
};

#endif // TCPCLIENT_H
