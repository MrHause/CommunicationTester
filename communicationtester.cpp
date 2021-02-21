#include "communicationtester.h"
#include "ui_communicationtester.h"
#include "serial.h"

CommunicationTester::CommunicationTester(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CommunicationTester)
{
    ui->setupUi(this);
    serialWidget = new Serial();
    ui->tabWidget->addTab(serialWidget, QString("Serial"));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

CommunicationTester::~CommunicationTester()
{
    delete ui;
}

void CommunicationTester::tabChanged(int index){
    if(index == 3){
        serialWidget->SerialInit();
    }
}

