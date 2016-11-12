#ifndef UINPUTWRAPPER_H
#define UINPUTWRAPPER_H

#include <QObject>

/**
 * @brief Qt wrapper for UInput
 */
class UInputWrapper : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief The BusType enum defines the bus' type
     */
    enum BusType {
        BusPCI          = 0x01,
        BusISAPNP       = 0x02,
        BusUSB          = 0x03,
        BusHIL          = 0x04,
        BusBluetooth    = 0x05,
        BusVirtual      = 0x06,
        BusISA			= 0x10,
        BusI8042		= 0x11,
        BusXTKBD		= 0x12,
        BusRS232		= 0x13,
        BusGamePort		= 0x14,
        BusParPort		= 0x15,
        BusAMIGA		= 0x16,
        BusADB			= 0x17,
        BusI2C			= 0x18,
        BusHhost        = 0x19,
        BusGSC			= 0x1A,
        BusAtari		= 0x1B,
        BusSPI			= 0x1C
    };

    /**
     * @brief The KeyState enum defines the state of a key
     */
    enum KeyState {
        KeyReleased = 0, /**< @brief Released state */
        KeyPressed = 1   /**< @brief Pressed state */
    };

    explicit UInputWrapper(QObject *parent = 0);
    ~UInputWrapper();

    /**
     * @brief Opens the uinput device (/dev/uinput)
     * @return true if succeeded; false otherwise
     */
    bool openDevice();

    /**
     * @brief Creates the device with the given parameters
     * @param name device's name
     * @param type device's type
     * @param vendor device's vendor id
     * @param product device's product id
     * @param version device's verion
     * @return true if succeeded; false otherwise.
     */
    bool createDevice(const QString &name,
                      BusType type,
                      uint vendor,
                      uint product,
                      uint version);

    /**
     * @brief Registers the given event
     * @param event event to be registed
     * @return true if succeeded; false otherwise.
     */
    bool registerEvent(unsigned int event);
    /**
     * @brief Registers the given key
     * @param key the key to be registred
     * @return true if succeeded; false otherwise
     */
    bool registerKey(unsigned int key);

    bool sendKeyEvent(uint keyCode, KeyState keyState);
    bool sendKeyPressedEvent(uint keyCode);
    bool sendKeyReleasedEvent(uint keyCode);
    bool sendKeyPressedAndReleasedEvent(uint keyCode);

private:
    int m_fd;
};

#endif // UINPUTWRAPPER_H
