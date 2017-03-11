///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Clock_Rotery_Switch
//
// 
// by Suusi Malcolm-Brown
//
/////////////////////////////////////////////////////////////////////////////////////////////////
//Wiring
 
//Connect SDA to SDA (A4) (Nano pin 27)
//Connect SCL to SCL (A5) (Nano pin 28)
//Connect GND to GND
//Connect VCC to VCC

// Include the Wire library for I2C access.
#include <Wire.h>
#include "I2C_lcd_definitions.h"
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
// download the repository from here and put it in your documents/arduino/libraries folder and restart your ide 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
/*
LiquidCrystal_I2C_ByVac.h

change from 
#include <Arduino.h>
to
#if (ARDUINO <  100)
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
// Include the rotary encoder library.
#include <RotaryEncoder.h>
// Include the RTC library.
#include "DS1307.h"
#include "menu.h"


int iOldSecond = 0;
int iPressCount = 0;
char  szTemp[41];                               // temp work aria for sprintf

// Variable to keep track of where we are in the "menu"

extern RotaryEncoder rotary;

// construct a Real Time Clock
DS1307 RTC;
// construct a I2C LCD
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

tm td;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  void setup ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////


void setup () {
  Serial.begin(115200);
  Wire.begin();
  
  RTC.Initialise();

  lcd.begin (20,4);  // initialize the lcd
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(LED_ON);

  lcd.clear();
  lcd.home();
    
  if (! RTC.IsRunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.Get( td ); 
    td.tm_year = 2017 ;           // years CE
    td.tm_mon  = 1;               // months  0..11
    td.tm_mday = 1;               // day     1..31
    RTC.Set( td );
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// void loop ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void loop () {
  RTC.Get( td ); 
  if (iOldSecond != td.tm_sec ){
    lcd.setCursor( 0, 0 ); 
    sprintf( szTemp, "%04d/%02d/%02d %02d:%02d:%02d", td.tm_year, td.tm_mon+1, td.tm_mday, td.tm_hour+1, td.tm_min, td.tm_sec);
    lcd.print( szTemp );
  }
  Menu();
 }


