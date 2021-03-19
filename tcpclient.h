#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>

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
    void buttonStartpressed();
    void timerTimeout();

private:
    Ui::TCPClient *ui;
    QTcpSocket *tcpSocket;
    QTimer *tcpTimer;
    bool connect_status;
    bool isAutoSend;
};

#endif // TCPCLIENT_H
