#include "communicationtester.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CommunicationTester w;
    w.show();
    return a.exec();
}
