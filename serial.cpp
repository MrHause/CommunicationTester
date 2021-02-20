#include "serial.h"
#include "ui_serial.h"

Serial::Serial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(buttonSetPressed()));
}

Serial::~Serial()
{
    delete ui;
}

void Serial::buttonSetPressed(){
    int a = 0;
    a = 10;
}
