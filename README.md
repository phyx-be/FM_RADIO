# FM_RADIO

A low cost, arduino compatible, stereo FM radio designed to be used easy in use for elder people. Rotating the rotary encoder wil increase or decrease the volume, pushing it wil enable of disable the radio. The station is pre-defined in software. The board does feature some expansion abilities. You could add more buttons to change stations. Or add an [SSD1306](https://www.adafruit.com/datasheets/SSD1306.pdf) based I²C display to have a nice user interface.

## Hardware Revision 00

The board is designed around the [ATMega 328p](www.atmel.com/devices/ATMEGA328P.aspx), the same microcontroller found on the Arduino Uno. The RRD-102 FM module is used as FM receiver and two NS4158 class D amplifiers are used to provide more than enought audio output. We added all the required components for a AR1019 FM receiver solution in case the RRD-102 would nog suffice.

![AUDIO_FM_LC_00_TOP](https://raw.githubusercontent.com/phyx-be/FM_RADIO/master/hw/AUDIO_FM_LC_00/3D_VIEW_TOP.PNG)
![AUDIO_FM_LC_00_BOT](https://raw.githubusercontent.com/phyx-be/FM_RADIO/master/hw/AUDIO_FM_LC_00/3D_VIEW_BOT.PNG)

### Known issues

RRD-102 FM module should be powered by a 3.3V power supply, not 5V
The LED on the SCK pin will make it impossible to load new firmware through ISP

## Hardware Revision 01

We moved the power input to the same side as the speaker output, removed the AR1019 solution, added a 1117 voltage regulator for the 3.3V signals. Converted the I2C signals to 3.3V and added an easy-to-use header to add a small SSD1306 based OLED display.

![AUDIO_FM_LC_01_TOP](https://raw.githubusercontent.com/phyx-be/FM_RADIO/master/hw/AUDIO_FM_LC_01/3D_VIEW_TOP.PNG)
![AUDIO_FM_LC_01_BOT](https://raw.githubusercontent.com/phyx-be/FM_RADIO/master/hw/AUDIO_FM_LC_01/3D_VIEW_BOT.PNG)
