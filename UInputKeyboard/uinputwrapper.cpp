#include "uinputwrapper.h"

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

UInputWrapper::UInputWrapper(QObject *parent)
    : QObject(parent)
    , m_fd(-1)
{

}

UInputWrapper::~UInputWrapper()
{
    if (ioctl(m_fd, UI_DEV_DESTROY) < 0)
        perror("error: ioctl");

    close(m_fd);
}

bool UInputWrapper::openDevice()
{
    m_fd = open("/dev/uinput", O_RDWR | O_NONBLOCK | O_NDELAY);

    if (m_fd < 0) {
        perror("error: open");
        return false;
    }

    return true;
}

bool UInputWrapper::createDevice(const QString &name, BusType type, uint vendor, uint product, uint version)
{
    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    strncpy(uidev.name, qPrintable(name), UINPUT_MAX_NAME_SIZE);
    uidev.id.bustype = type;
    uidev.id.vendor  = vendor;
    uidev.id.product = product;
    uidev.id.version = version;

    if (write(m_fd, &uidev, sizeof(uidev)) < 0) {
        perror("error: write");
        return false;
    }

    if (ioctl(m_fd, UI_DEV_CREATE) < 0) {
        perror("error: ioctl UI_DEV_CREATE");
        return false;
    }

    return true;
}

bool UInputWrapper::registerEvent(unsigned int e)
{
    if (ioctl(m_fd, UI_SET_EVBIT, e) < 0) {
        perror("error: ioctl UI_SET_EVBIT");
        return false;
    }
    return true;
}

bool UInputWrapper::registerKey(unsigned int key)
{
    if (ioctl(m_fd, UI_SET_KEYBIT, key) < 0) {
        perror("error: ioctl UI_SET_KEYBIT");
        return false;
    }
    return true;
}

bool UInputWrapper::sendKeyEvent(uint keyCode, KeyState keyState)
{
    struct input_event event;

    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = keyCode;
    event.value = keyState;
    write(m_fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(m_fd, &event, sizeof(event));

    return true;
}

bool UInputWrapper::sendKeyPressedEvent(uint keyCode)
{
    return sendKeyEvent(keyCode, KeyPressed);
}

bool UInputWrapper::sendKeyReleasedEvent(uint keyCode)
{
    return sendKeyEvent(keyCode, KeyReleased);
}

bool UInputWrapper::sendKeyPressedAndReleasedEvent(uint keyCode)
{
    if (sendKeyPressedEvent(keyCode))
        return sendKeyReleasedEvent(keyCode);

    return false;
}
