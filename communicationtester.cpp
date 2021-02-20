#include "communicationtester.h"
#include "ui_communicationtester.h"
#include "serial.h"

CommunicationTester::CommunicationTester(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CommunicationTester)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new Serial(), QString("Serial"));
}

CommunicationTester::~CommunicationTester()
{
    delete ui;
}

