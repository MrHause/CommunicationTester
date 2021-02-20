#ifndef COMMUNICATIONTESTER_H
#define COMMUNICATIONTESTER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CommunicationTester; }
QT_END_NAMESPACE

class CommunicationTester : public QMainWindow
{
    Q_OBJECT

public:
    CommunicationTester(QWidget *parent = nullptr);
    ~CommunicationTester();

private:
    Ui::CommunicationTester *ui;
};
#endif // COMMUNICATIONTESTER_H
