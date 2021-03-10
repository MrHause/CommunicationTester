#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include "QtSerialPort"
#include "QSerialPortInfo"

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
    void readSerialData();

private:
    Ui::Serial *ui;
    QSerialPort *m_serial = nullptr;
    void fillParameters();
    void updateSettings();

};

#endif // SERIAL_H
