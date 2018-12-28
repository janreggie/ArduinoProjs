/*
  On the usage of hardcoding a clock in an Arduino
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);  // set LCD input desu~ne

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

RTC_DS1307 rtc;

const char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const int daysInMonth [] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // note that 0 is January!

const byte S1 = 2;  // enter mode
const byte S2 = 3;  // increment current shitsu
char mode = 0;  // 0 if nothin; 1 HOUR; 2 MINUTE; 3 SECOND; 4 MONTH; 5 DAY; 6 YEAR
DateTime nun;
byte isEditing = 0;  // 0 if not "incrementMe'ing", 1 otherwise

void setup() {

  /*
     Set up RTC comms.
  */
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Serial.println("RTC is NOT running!");
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));


  Serial.println("Setup ran successfully desu~!\n");

  /*
     Set up LED shits
  */

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("IT'S");
  lcd.setCursor(0, 1);
  lcd.print("HAMMER TIME");

  delay(1000);
  attachInterrupt(digitalPinToInterrupt(S1), changeMode, FALLING);
  attachInterrupt(digitalPinToInterrupt(S2), incrementMe, FALLING);
}

void printTimeNow() {
  /*
     Print style:
     HH:MM:SS AM/PM
     MM/DD/YYYY DAY_OF_THE_WEEK
  */
  char toPrint[16];  // the thing to be printed no shit sherlock
  lcd.clear();
  nun = rtc.now();  // now

  // determine whether AM or PM;
  // UGGGGGHHHH
  int hrme;
  if (nun.hour() < 12) {  // i.e. AM
    if (nun.hour() == 0) hrme = 12;
    else hrme = nun.hour();
    sprintf(toPrint, "%02d:%02d:%02d AM %d %d", hrme, nun.minute(), nun.second(), mode, isEditing);
  }
  else {
    if (nun.hour() == 12) hrme = 12;
    else hrme = (nun.hour() - 12);
    sprintf(toPrint, "%02d:%02d:%02d PM %d %d", hrme, nun.minute(), nun.second(), mode, isEditing);
  }
  Serial.print(toPrint);
  Serial.print('\n');
  lcd.setCursor(0, 0);
  lcd.print(toPrint);

  // now print the date
  sprintf(toPrint, "%02d/%02d/%04d %s", nun.month(), nun.day(), nun.year(), daysOfTheWeek[nun.dayOfTheWeek()]);
  Serial.print(toPrint);
  Serial.print('\n');
  lcd.setCursor(0, 1);
  lcd.print(toPrint);

  free(toPrint);
}

void changeMode() {
  mode += 1;
  if (mode == 7) mode = 0;
}

void incrementMe() {  // INCREMENT
  isEditing = 1;  // that's it lol.
}

void loop() {
  /*
     Display the time
  */
  printTimeNow();
  delay(250);
  // do some editing
  if (isEditing) {
    if (mode == 1) rtc.adjust(nun + TimeSpan(0, 1, 0, 0)); // inc hour
    else if (mode == 2) rtc.adjust(nun + TimeSpan(0, 0, 1, 0)); // inc minute
    else if (mode == 3) rtc.adjust(nun + TimeSpan(0, 0, 0, 1)); // inc second
    else if (mode == 4) rtc.adjust(nun + TimeSpan(daysInMonth[nun.month() - 1], 0, 0, 0)); // inc month
    else if (mode == 5) rtc.adjust(nun + TimeSpan(1, 0, 0, 0)); // inc day
    else if (mode == 6) {  // inc year is special ;)
      auto yr = nun.year();
      // okay apparently upon inspection of the library
      // i don't need to do the cases when year is divisible by 100 but not by 400
      // oh fucking well i guess
      if (yr % 4 == 0 && !(yr % 100 == 0 && yr % 400 != 0))
        rtc.adjust(nun + TimeSpan(366, 0, 0, 0)); // believe me this works
      else
        rtc.adjust(nun + TimeSpan(365, 0, 0, 0));
    }
    Serial.print("That's some successful editing.");
    isEditing = 0;
    delay(250);
    return;
  }
}
