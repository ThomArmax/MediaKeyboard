#include "HID-Project.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
volatile bool clearAndUpdate = true;    /**< @brief Used in loop to tells if the screen should be cleared and updated */

// Debounce parameters (expressed in ms)
unsigned long lastDebounceTime = 0;     /**< @brief The last time rotary encoder switch button was toggled */
unsigned long debounceDelay = 250;      /**< @brief The debounce time */

bool isMediaPlayerDetected = false;     /**< @brief Tells if a media player hase been detected or not */

// Forward declarations
void doEncoder_Expanded();
void doEncoder();
void doClick();

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
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

void showToastInfo(const char* string) {
    display.clearDisplay();
    display.setCursor(30, 15);
    display.println(string);
    display.display();
    clearAndUpdate = false;
}

void loop() {
    // Toast time elapsed
    unsigned long currentTime = millis();
    if ((currentTime - lastToastTime) > toastDuration) {
        lastToastTime = millis();
        clearAndUpdate = true;
    }

    if (!isMediaPlayerDetected && clearAndUpdate) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("No media player");
        display.println("detected");
        display.display();
    }

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
    delay(50);
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
