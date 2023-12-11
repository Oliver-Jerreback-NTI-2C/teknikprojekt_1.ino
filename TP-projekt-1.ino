/* File name: teknikprojekt_1.ino
 * Author: Oliver Jerrebäck
 * Date:2023-11-29
 * Description: Its a pulssensor that will give results depending on the puls and gender. 
 */

// Include necessary libraries
#define USE_ARDUINO_INTERRUPTS true    // Enable low-level interrupts for accurate BPM calculation.
#include <PulseSensorPlayground.h>     // Include the PulseSensorPlayground Library.
#include "U8glib.h"                    // Include the screen library
#include <Wire.h>

// Initialize OLED screen object
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);
boolean state = true;  // Variable to toggle between states

// Variables for PulseSensor
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 800;           // Threshold to determine a beat, can be fine-tuned

PulseSensorPlayground pulseSensor;  // Create an instance of the PulseSensorPlayground object

// Function to update the OLED display
void updateOled(String text1, String text2);

void setup() {
  pinMode(2, INPUT);  // Define pin 2 as INPUT for state toggling
  oled.setFont(u8g_font_helvB10);  // Set the font for the OLED display
  Serial.begin(9600);  // Initialize serial communication for debugging

  // Configure the PulseSensor object
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);  // Set the beat detection threshold

  // Check if the PulseSensor object was created and is detecting a signal
  if (pulseSensor.begin()) {
    Serial.println("PulseSensor Object Created!");  
  }
}

void loop() {
  // Debugging: Print analog sensor value to Serial Monitor
  Serial.println(analogRead(A0));

  // Toggle state when button is pressed (pin 2)
  if (digitalRead(2) == 0) {
    state = !state;
  }

  // Check for a heartbeat using PulseSensor
  if (pulseSensor.sawStartOfBeat()) {
    int myBPM = pulseSensor.getBeatsPerMinute();  // Get the BPM value from the PulseSensor

    // Check the state for display customization
    if (state) {
      int adjustedBPM = myBPM;

      // Update OLED display based on BPM range
      if (adjustedBPM < 60) {
        updateOled("BPM Kille: " + String(myBPM), "LOW BPM");
      } else if (adjustedBPM >= 60 && adjustedBPM < 110) {
        updateOled("BPM Kille: " + String(myBPM), "NORMAL BPM");
      } else {
        updateOled("BPM Kille: " + String(myBPM), "HIGH BPM");
      }
    } else {
      int adjustedBPM = myBPM;

      // Update OLED display based on BPM range
      if (adjustedBPM < 60) {
        updateOled("BPM Tjej: " + String(myBPM + 10), "LOW BPM");
      } else if (adjustedBPM >= 60 && adjustedBPM < 110) {
        updateOled("BPM Tjej: " + String(myBPM + 10), "NORMAL BPM");
      } else {
        updateOled("BPM Tjej: " + String(myBPM + 10), "HIGH BPM");
      }
    }

    delay(40); // Delay for display readability
  }
}

// Function to update the OLED display with two lines of text
void updateOled(String text1, String text2) {
  oled.firstPage();
  do {
    oled.drawStr(10, 20, text1.c_str());  // Draw the first line at coordinates (10, 20)
    oled.drawStr(10, 40, text2.c_str());  // Draw the second line at coordinates (10, 40)
  } while (oled.nextPage());
}
