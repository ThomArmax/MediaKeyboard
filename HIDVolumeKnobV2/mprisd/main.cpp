#include <QCoreApplication>

#include "mpriswatcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MPRISWatcher watcher;
    watcher.start();

    return a.exec();
}
