///
/// \file  TestRDA5807M.ino
/// \brief An Arduino sketch to operate a SI4705 chip based radio using the Radio library.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2014 by Matthias Hertel.\n
/// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
///
/// \details
/// This sketch implements a "as simple as possible" radio without any possibility to modify the settings after initializing the chip.\n
/// The radio chip is initialized and setup to a fixed band and frequency. These settings can be changed by modifying the 
/// FIX_BAND and FIX_STATION definitions. 
///
/// Open the Serial console with 57600 baud to see the current radio information.
///
/// Wiring
/// ------ 
/// The RDA5807M board/chip has to be connected by using the following connections:
/// | Arduino UNO pin    | Radio chip signal  | 
/// | -------------------| -------------------| 
/// | 3.3V (red)         | VCC                | 
/// | GND (black)        | GND                | 
/// | A5 or SCL (yellow) | SCLK               | 
/// | A4 or SDA (blue)   | SDIO               | 
/// The locations of the pins on the UNO board are written on the PCB.
/// The locations of the signals on the RDA5807M side depend on the board you use.
///
/// More documentation and source code is available at http://www.mathertel.de/Arduino
///
/// ChangeLog:
/// ----------
/// * 05.12.2014 created.
/// * 19.05.2015 extended.

#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <RDA5807M.h>

// ----- Fixed settings here. -----

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION  8930            ///< The station that will be tuned by this sketch is 89.30 MHz.
#define ENCODER_A       1            ///< Encoder pin A (A1)
#define ENCODER_B       2            ///< Encoder pin B (A2)
#define ENCODER_BUTTON  3            ///< Encoder button (A3)
#define VOLUME_LEVELS   16
#define PULSE_PER_VOLUME  3

int encoder0Pos = 0;
int encoder0PinALast = LOW;
int currentVolume = 0;

RDA5807M radio;    // Create an instance of Class for RDA5807M Chip

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup() {
  // open the Serial port
  Serial.begin(57600);
  Serial.println("Radio...");
  delay(200);

  // Initialize the Radio 
  radio.init();

  // Enable information to the Serial port
  radio.debugEnable();

  // Set all radio setting to the fixed values.
  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  radio.setVolume(currentVolume);
  radio.setMono(false);
  radio.setMute(false);

  //setup encoder
  pinMode (ENCODER_A,INPUT);
  pinMode (ENCODER_B,INPUT);
  
} // setup


/// show the current chip data every 3 seconds.
void loop() {
  char s[12];
  radio.formatFrequency(s, sizeof(s));
  Serial.print("Station:"); 
  Serial.println(s);
  
  Serial.print("Radio:"); 
  radio.debugRadioInfo();
  
  Serial.print("Audio:"); 
  radio.debugAudioInfo();

  int n = digitalRead(ENCODER_A);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(ENCODER_B) == LOW) {
      if(encoder0Pos > 0) encoder0Pos--;
    } else {
      if(encoder0Pos < (VOLUME_LEVELS*PULSE_PER_VOLUME)) encoder0Pos++;
    }
  } 
  encoder0PinALast = n;
  if((encoder0Pos/PULSE_PER_VOLUME) != currentVolume) {
    currentVolume = encoder0Pos/PULSE_PER_VOLUME;
    radio.setVolume(currentVolume);
  }
} // loop

// End.
