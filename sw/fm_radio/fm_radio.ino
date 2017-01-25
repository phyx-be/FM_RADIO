/// An Arduino sketch to operate a RDA5807M chip based radio using the Radio library of Matthias Hertel, http://www.mathertel.de.
/// This sketch implements a fixed radio station. The volume can be controlled using a rotary encoder.
///
/// Bert Outtier
/// Copyright (c) 2017 by Bert Outtier.

#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <RDA5807M.h>

// ----- Fixed settings here. -----
#define FIX_BAND          RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION       8800            ///< The station that will be tuned by this sketch is 89.30 MHz.
#define AMP_ENABLE_PIN    A0              ///< Amp enable pin (A0)
#define ENCODER_A         A2              ///< Encoder pin A (A1)
#define ENCODER_B         A1              ///< Encoder pin B (A2)
#define ENCODER_BUTTON    A3              ///< Encoder button (A3)
#define VOLUME_LEVELS     16
#define PULSE_PER_VOLUME  1

int encoder0Pos = 0;            // encoder value (0 to VOLUME_LEVELS*PULSE_PER_VOLUME)
int encoder0PinALast = LOW;     // previous value of encoder pin A
int currentVolume = 0;          // Current volume level
int previousAmpState = LOW;     // previous amp state (enabled/disabled)
int currentAmpState = LOW;     // current amp state (enabled/disabled)
int previousButtonPinState = LOW;  // previous button state (pressed/not pressed)
int currentButtonState = LOW;
int previousRadioState = HIGH;  // debounced previous button state (pressed/not pressed)
int currentRadioState = HIGH;   // is the radio on or off?
long time = 0;                  // the last time the encoder button pin was toggled
long debounce = 100;            // the debounce time

RDA5807M radio;    // Create an instance of Class for RDA5807M Chip

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup() {
  // open the Serial port
  Serial.begin(9600);
  Serial.println("Radio...");
  delay(200);

  //setup encoder
  pinMode (AMP_ENABLE_PIN, OUTPUT);
  pinMode (ENCODER_A,INPUT_PULLUP);
  pinMode (ENCODER_B,INPUT_PULLUP);
  pinMode (ENCODER_BUTTON,INPUT_PULLUP);

  digitalWrite(AMP_ENABLE_PIN, currentAmpState);

  // Initialize the Radio 
  radio.init();

  // Enable information to the Serial port
  //radio.debugEnable();

  // Set all radio setting to the fixed values.
  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  radio.setVolume(currentVolume);
  radio.setMono(false);
  radio.setMute(false);

  Serial.println("Init done!");
  
} // setup


/// show the current chip data every 3 seconds.
void loop() {
//  char s[12];
//  radio.formatFrequency(s, sizeof(s));
//  Serial.print("Station:"); 
//  Serial.println(s);
//  
//  Serial.print("Radio:"); 
//  radio.debugRadioInfo();
//  
//  Serial.print("Audio:"); 
//  radio.debugAudioInfo();

  int n = digitalRead(ENCODER_A);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(ENCODER_B) == LOW) {
      if(encoder0Pos > 0) encoder0Pos--;
    } else {
      if(encoder0Pos < (VOLUME_LEVELS*PULSE_PER_VOLUME)) encoder0Pos++;
    }
    Serial.print("Set encoder pos ");
    Serial.println(encoder0Pos, DEC);
  } 
  encoder0PinALast = n;
  
  if((encoder0Pos/PULSE_PER_VOLUME) != currentVolume) {
    currentVolume = encoder0Pos/PULSE_PER_VOLUME;
    Serial.print("Set volume to ");
    Serial.println(currentVolume, DEC);
    radio.setVolume(currentVolume);
  }

    // read the state of the switch into a local variable:
  int reading = digitalRead(ENCODER_BUTTON);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != previousButtonPinState) {
    // reset the debouncing timer
    time = millis();
  }

  if ((millis() - time) > debounce) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != currentButtonState) {
      currentButtonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (currentButtonState == HIGH) {
        currentRadioState = !currentRadioState;
        Serial.print("Set radio state: ");
        Serial.println(currentRadioState, DEC);
      }
    }
  }
  previousButtonPinState = reading;
  
  if(currentVolume == 0 || !currentRadioState){
    currentAmpState = LOW;
  }

  if(currentVolume > 0 && currentRadioState){
    currentAmpState = HIGH;
  }

  if(previousAmpState != currentAmpState){
    Serial.print("Set amp: ");
    Serial.println(currentAmpState, DEC);
    digitalWrite(AMP_ENABLE_PIN, currentAmpState);
    previousAmpState = currentAmpState;
  }
} // loop

// End.
