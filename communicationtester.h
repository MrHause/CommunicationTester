#ifndef COMMUNICATIONTESTER_H
#define COMMUNICATIONTESTER_H

#include <QMainWindow>
#include <serial.h>

QT_BEGIN_NAMESPACE
namespace Ui { class CommunicationTester; }
QT_END_NAMESPACE

class CommunicationTester : public QMainWindow
{
    Q_OBJECT

public:
    CommunicationTester(QWidget *parent = nullptr);
    ~CommunicationTester();

private slots:
    void tabChanged(int index);

private:
    Ui::CommunicationTester *ui;
    Serial *serialWidget;

};
#endif // COMMUNICATIONTESTER_H
