#include <Arduino.h>

#include "keys.h"
#include "pinconfig.h"

volatile unsigned int encoder0Pos = 0;  /**< @brief Rotary encore position */
volatile unsigned int key = 0;          /**< @brief Key code to be sent */
volatile bool keySent = false;          /**< @brief Key send state */

// Forward declarations
void doEncoder_Expanded();
void doEncoder();
void doClick();

using namespace VolumeKnob;

/**
 * @brief Hardware setup
 */
void setup()
{
    // Encoder configuration
    pinMode(encoder0PinA, INPUT);
    digitalWrite(encoder0PinA, HIGH);
    pinMode(encoder0PinB, INPUT);
    digitalWrite(encoder0PinB, HIGH);

    // Encoder push button configuration
    pinMode(encoderButton, INPUT);
    digitalWrite(encoderButton, HIGH);

    // Interrupts configuration
    attachInterrupt(0, doEncoder, CHANGE);
    attachInterrupt(1, doClick, CHANGE);

    Serial.begin(9600);
}

void loop()
{
    if (!keySent) {
        Serial.write(key);
        keySent = true;
    }
    delay(50);
}

/**
 * @brief Rotary encoder push button interrupt handler
 */
void doClick()
{
    if (digitalRead(encoderButton) == LOW) {
        key = KEY_PLAYPAUSE;
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
        key = KEY_VOLUMEUP;
    }
    else {
        encoder0Pos--;
        key = KEY_VOLUMEDOWN;
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
            key = KEY_VOLUMEDOWN;
        }
        else {
            encoder0Pos = encoder0Pos + 1;          // CW
            key = KEY_VOLUMEUP;
        }
    }
    else {                                          // found a high-to-low on channel A
        if (digitalRead(encoder0PinB) == LOW) {     // check channel B to see which way
            encoder0Pos = encoder0Pos + 1;          // CW
            key = KEY_VOLUMEUP;
        }
        else {
            encoder0Pos = encoder0Pos - 1;          // CCW
            key = KEY_VOLUMEDOWN;
        }
    }

    keySent = false;
}
