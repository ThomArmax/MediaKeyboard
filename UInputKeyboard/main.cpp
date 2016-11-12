#include <QCoreApplication>

#include "mediakeyboardreceiver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MediaKeyBoardReceiver mediaKeyboard;
    mediaKeyboard.listen("/dev/ttyUSB0", 9600);

    return a.exec();
}
