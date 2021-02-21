#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>

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


private:
    Ui::Serial *ui;
};

#endif // SERIAL_H
