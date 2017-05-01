#include <HID-Project.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "oleddata.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

static const unsigned int encoder0PinA  = 0;
static const unsigned int encoder0PinB  = 1;
static const unsigned int encoderButton = 7;

// Toast management
static const unsigned int toastDuration = 1500;
unsigned long lastToastTime = 0;

volatile unsigned int encoder0Pos = 0;  /**< @brief Rotary encore position */
volatile unsigned int key = 0;          /**< @brief Key code to be sent */
volatile bool keySent = false;          /**< @brief Key send state */
volatile bool isInToastMode = true;     /**< @brief Tells if we are in 'toast' mode. If yes, the meta data are not displayed */

// Debounce parameters (expressed in ms)
unsigned long lastDebounceTime = 0;     /**< @brief The last time rotary encoder switch button was toggled */
unsigned long debounceDelay = 250;      /**< @brief The debounce time */

bool isMediaPlayerDetected = false;     /**< @brief Tells if a media player hase been detected or not */

// Forward declarations
void doEncoder_Expanded();
void doEncoder();
void doClick();

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Meta data
/**
 * @brief olds the meta datas
 */
char* metaDatas[4] = { NULL, NULL, NULL, NULL };

void setup()
{
    Consumer.begin();

    // Encoder configuration
    pinMode(encoder0PinA, INPUT);
    digitalWrite(encoder0PinA, HIGH);
    pinMode(encoder0PinB, INPUT);
    digitalWrite(encoder0PinB, HIGH);

    // Encoder push button configuration
    pinMode(encoderButton, INPUT);
    digitalWrite(encoderButton, HIGH);

    // Interrupts configuration
    attachInterrupt(2, doEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoderButton), doClick, FALLING);

    Serial.begin(9600);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

    // Clear the buffer.
    display.clearDisplay();
    // text display tests
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("HID Volume Knob V2");
    display.setCursor(0, 10);
    display.println("V 0.1");
    display.display();

    delay(2000);
    display.clearDisplay();
    display.display();
}

/**
 * @brief Displays the given string as a 'toast'
 * @param string string to be displayed
 */
void showToastInfo(const char* string)
{
    display.clearDisplay();
    display.setCursor(30, 15);
    display.println(string);
    display.display();
    isInToastMode = true;
}

/**
 * @brief Show the meta datas
 */
void showMetaData()
{
    display.clearDisplay();
    for (int i = 0; i < 4; i++) {
        char* data = metaDatas[i];
        int cursorPosition = 8 * i;
        display.setCursor(0, cursorPosition);
        display.println(data);
    }
    display.display();
}

/**
 * @brief Sets the meta data for the given index
 * @param index
 * @param data
 */
void setMetaData(int index, const char *data)
{
    if (metaDatas[index])
        free(metaDatas[index]);
    metaDatas[index] = (char*)malloc(strlen(data) * sizeof(char));
    strcpy(metaDatas[index], data);
}

void loop()
{
    // Toast time elapsed
    unsigned long currentTime = millis();
    if ((currentTime - lastToastTime) > toastDuration) {
        lastToastTime = millis();
        isInToastMode = false;
    }

    int availableBytes = Serial.available();
    if (availableBytes) {
        isMediaPlayerDetected = true;
        char *data = (char *)malloc(availableBytes * sizeof(char));
        Serial.readBytes(data, availableBytes);
        OLEDData oledData;
        if (OLEDData::decode(data, availableBytes, &oledData)) {
            //display.clearDisplay();
            switch (oledData.type()) {
            case OLEDData::Artist:
                setMetaData(0, oledData.data());
                break;
            case OLEDData::Album:
                setMetaData(1, oledData.data());
                break;
            case OLEDData::Track:
                setMetaData(2, oledData.data());
                break;
            default:
                break;
            }
        }
        free(data);
    }

    if (!isInToastMode)
        showMetaData();

    if (!keySent) {
        Consumer.write(key);
        keySent = true;

        if (key == MEDIA_PLAY_PAUSE)
            showToastInfo("PLAY/PAUSE");
        else if (key == MEDIA_VOLUME_UP)
            showToastInfo("VOLUME UP");
        else if (key == MEDIA_VOLUME_DOWN)
            showToastInfo("VOLUME DOWN");
    }
}

/**
 * @brief Rotary encoder push button interrupt handler
 */
void doClick()
{
    unsigned long currentTime = millis();
    if ((currentTime - lastDebounceTime) > debounceDelay) {
        key = MEDIA_PLAY_PAUSE;
        lastDebounceTime = millis();
        keySent = false;
    }
}

/**
 * @brief Rotary encoder interrupt handler
 */
void doEncoder()
{
    /* If pinA and pinB are both high or both low, it is spinning
     * forward. If they're different, it's going backward.
     *
     * For more information on speeding up this process, see
     * [Reference/PortManipulation], specifically the PIND register.
     */
    if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
        encoder0Pos++;
        key = MEDIA_VOLUME_UP;
    }
    else {
        encoder0Pos--;
        key = MEDIA_VOLUME_DOWN;
    }
    keySent = false;
}

/**
 * @brief Rotary encoder interrupt handler
 */
void doEncoder_Expanded()
{
    if (digitalRead(encoder0PinA) == HIGH) {        // found a low-to-high on channel A
        if (digitalRead(encoder0PinB) == LOW) {     // check channel B to see which way
            encoder0Pos = encoder0Pos - 1;          // CCW
            key = MEDIA_VOLUME_DOWN;
        }
        else {
            encoder0Pos = encoder0Pos + 1;          // CW
            key = MEDIA_VOLUME_UP;
        }
    }
    else {                                          // found a high-to-low on channel A
        if (digitalRead(encoder0PinB) == LOW) {     // check channel B to see which way
            encoder0Pos = encoder0Pos + 1;          // CW
            key = MEDIA_VOLUME_UP;
        }
        else {
            encoder0Pos = encoder0Pos - 1;          // CCW
            key = MEDIA_VOLUME_DOWN;
        }
    }

    keySent = false;
}
