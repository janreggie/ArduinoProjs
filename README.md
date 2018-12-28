# ArduinoProjs: Arduino Projects for ELC 141.2

This repository contains all the projects the uploader has performed in their ELC 141.2 (Microprocessor Systems: Laboratory) class. All projects are in the form of INO files, to be uploaded to an Arduino.

The Arduino used is a clone of the Arduino Uno, used with an [e-Gizmo Universal MCU Trainer](http://e-gizmo.net/oc/kits%20documents/Universal%20Microcontroller%20Trainer%20Board/Trainer_Hardware.pdf). External libraries are used. Before uploading, it is made sure that the code is safe for work.

All code is released under the Unlicense.

## ADVANCEDHARDCODING.INO: Interfacing Real Time Clock with LCD Display

The program reads from the RTC connected on the Universal MCU Trainer and prints the output to the 16x2 LCD display. The pin-out is as follows:

* D12 to RS pin of LCD display
* D11 to ENABLE pin of LCD display
* D4, D5, D6, and D7 to data lines d4, d5, d6, and d7 of the LCD display
* A5 to SCL of RTC
* A4 to SDA of RTC

External libraries used are [RTClib](https://github.com/adafruit/RTClib) and [LiquidCrystal](https://www.arduino.cc/en/Reference/LiquidCrystal).

## AHASNGBAYAN.INO: Arduino-implemented Snake Game

 The project aims to recreate the popular Snake game for Android, using a push button and an analogue joystick as inputs, and a color LCD shield as output. The push button serves as a control to start (or restart) the gameplay, whereas the joystick controls the snake for eating the fruit and avoiding the obstacles. The color LCD shield is the one used in [this guide](https://www.sparkfun.com/tutorials/286).

The software is written entirely from scratch, using two external libraries [SparkFunColorLCDShield](https://github.com/sparkfun/SparkFun_Color_LCD_Shield_Arduino_Library) and [LinkedList](https://github.com/ivanseidel/LinkedList). The code is extendible, allowing the user to change the colors (or use the dark theme), change the obstacles, modify the controls (e.g., using push buttons instead of a joystick), among other things.

The pin-out is as follows:
* D2 for the push-button "S1"
* D8 for RES (reset) of the color LCD
* D9 for CS (chip select) of the color LCD
* D11 for DIO (data I/O) of the color LCD
* D13 for the serial clock of the color LCD
* A0 and A1 for JOYX and JOYY of the analogue joystick

## TALAPINDUTAN.ino: Arduino Basic Two-digit Calculator using Keypad and LCD Display

This program aims to create a simple calculator for the Arduino, containing only the four basic operations and two-digit integer operands. The result is always displayed on the LCD display, and division by zero is always caught.

External libraries used are [LiquidCrystal](https://www.arduino.cc/en/Reference/LiquidCrystal) and [Keypad](https://github.com/Chris--A/Keypad). The pin-out is as follows:

* D12 to RS pin of LCD display
* D11 to ENABLE pin of LCD display
* D4, D5, D6, and D7 to data lines d4, d5, d6, and d7 of the LCD display
* D14, D15, D16, and D17 to ROW0, ROW1, ROW2, and ROW3 of the keypad
* D10, D9, and  D8 to COL0, COL1, and COL2 of the keypad
