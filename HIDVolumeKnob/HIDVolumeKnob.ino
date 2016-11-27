
#include "HID-Project.h"

static const unsigned int encoder0PinA  = 0;
static const unsigned int encoder0PinB  = 1;
static const unsigned int encoderButton = 2;

volatile unsigned int encoder0Pos = 0;  /**< @brief Rotary encore position */
volatile unsigned int key = 0;          /**< @brief Key code to be sent */
volatile bool keySent = false;          /**< @brief Key send state */

// Debounce parameters (expressed in ms)
unsigned long lastDebounceTime = 0;     /**< @brief The last time rotary encoder switch button was toggled */
unsigned long debounceDelay = 250;      /**< @brief The debounce time */

// Forward declarations
void doEncoder_Expanded();
void doEncoder();
void doClick();

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
  attachInterrupt(1, doClick, FALLING);

  Serial.begin(9600);
}

void loop() {
  if (!keySent) {
    Consumer.write(key);
    keySent = true;
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
