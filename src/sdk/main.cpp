#include <QCoreApplication>
#include <QTranslator>
#include <QThread>

#include "handler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Handler::instance()->init();
    Handler::instance()->dialBroker("172.16.255.130", 10086);
    QThread::msleep(500);
    Handler::instance()->quit();

    return a.exec();
}
