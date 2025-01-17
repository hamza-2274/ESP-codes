#include <Arduino.h>

// Define the GPIO pin for the LED
const int ledPin = D2; // Replace with the GPIO pin you are using

// Variables for PWM control
int brightness = 0;      // Current brightness level (0-1023)
int fadeAmount = 5;      // Amount to increase or decrease brightness

void setup() {
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Set the brightness of the LED using PWM
  analogWrite(ledPin, brightness);

  // Adjust the brightness for the next loop
  brightness += fadeAmount;

  // Reverse direction when the brightness reaches the limits
  if (brightness <= 0 || brightness >= 1023) {
    fadeAmount = -fadeAmount; // Reverse fade direction
  }

  // Delay to slow down the fading effect
  delay(20); // Increase this value (e.g., to 50 or 100) for a slower effect
}
