#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include "QtSerialPort"
#include "QSerialPortInfo"
#include "QTimer"

namespace Ui {
class Serial;
}

class Serial : public QWidget
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = nullptr);
    ~Serial();
    void SerialInit();

private slots:
    void buttonSetPressed();
    void buttonSendPressed();
    void buttonStartPressed();
    void readSerialData();
    void timerTimeout();

private:
    bool portIsOpened;
    bool isAutoSend;
    Ui::Serial *ui;
    QTimer *timer;
    QSerialPort *m_serial = nullptr;
    void fillParameters();
    void updateSettings();

};

#endif // SERIAL_H
