#include <QCoreApplication>

#include "serialconnection.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SerialConnection connection;

    return a.exec();
}
