//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// menus.cpp
//
// Date:      2017/03/9
// By:        Suusi Malcolm-Brown
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "menu.h"
// Include the rotary encoder library.
#include <RotaryEncoder.h>
// Include the RTC library.
#include "DS1307.h"
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

// construct a Real Time Clock
extern DS1307 RTC;
// construct a rotory encoder
RotaryEncoder rotary(6, 7, 5);

extern tm td;
extern LiquidCrystal_I2C  lcd;
char szMenuTemp[41];

int iMenu[8] = {  MENUSTATE_SETUP,      // main menu
                  MENUSTATE_SETUP,      // year menu
                  MENUSTATE_SETUP,      // month menu
                  MENUSTATE_SETUP,      // day menu
                  MENUSTATE_SETUP,      // hour menu
                  MENUSTATE_SETUP,      // minute menu
                  MENUSTATE_SETUP,      // second menu
                  MENUSTATE_SETUP       // day of tthe week menu
};

int iMenuCounter = 0;
int iActiveMenu = -1;                           // no active menu yet.
const char* pszMenu[]   = { "Year  ", "Month ", "Day   ", "Hour  ", "Minute", "Second", "DOW   "};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int Menu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu () {  
  if( iActiveMenu == -1 ) {
    MainMenu();
  }
  switch (iActiveMenu){
    case 0:                               // set year 
      YearMenu();
      break ;
    case 1:                               // set month 
      MonthMenu();
      break ;
    case 2:                               // set day of month
      DayMenu();
      break ;
    case 3:                               // set hours
      HourMenu();
      break ;
    case 4:                               // set minuites
      MinuteMenu();
      break;
    case 5:                               // set seconds
      SecondMenu();
      break;
    case 6:                               // set day of week (not used)
      break;
    default :
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int MainMenu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int MainMenu () {
  int iReturn = -1;
  switch( iMenu[MENU_MAIN] ){
    case MENUSTATE_SETUP:                                // normal state menu not in use
      rotary.minimum(0);
      rotary.maximum(6);
      rotary.position(0);
      iMenu[MENU_MAIN] = MENUSTATE_WAIT_PRESS;
      break;
    case MENUSTATE_WAIT_PRESS:                           // normal state menu not in use
      if( rotary.pressed()) {
        iMenu[MENU_MAIN] = MENUSTATE_WAIT_RELEASE;       // button relesed
      }
      break;
    case MENUSTATE_WAIT_RELEASE:                         // Rotery button pressed  
      if( !rotary.pressed()) { 
        iMenu[MENU_MAIN] = MENUSTATE_INITIALRELEASE ;    // button relesed
      }
      break;
    case MENUSTATE_INITIALRELEASE:                       // rotery button released menu active
      iMenuCounter = rotary.position();
      lcd.setCursor( 0, 1 ); 
      lcd.print( pszMenu[iMenuCounter] );
      if( rotary.pressed()) {
        iMenu[MENU_MAIN] = MENUSTATE_DONE;
       }
      break;
    case MENUSTATE_DONE:
      if( !rotary.pressed()) { 
        iMenu[MENU_MAIN] = MENUSTATE_SETUP;
        iActiveMenu = rotary.position();
        iReturn = iActiveMenu ;
      }
      break;
    default:
      break;
  }
 return iReturn;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int YearMenu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int YearMenu () {
  int iReturn = -1;
  int iYear ;
  switch( iMenu[MENU_YEAR] ){
    case MENUSTATE_SETUP:                             // rotery button released menu active
      rotary.minimum(2000);
      rotary.maximum(2050);
      RTC.Get( td );
      rotary.position(2017);
      iMenu[MENU_YEAR] = MENUSTATE_WAIT_PRESS;        // we have set up the rotery switch 
      break;
    case MENUSTATE_WAIT_PRESS:                        // we have set up the  
      lcd.setCursor( 0, 1 ); 
      sprintf( szMenuTemp, "Year: %04d", rotary.position());
      lcd.print( szMenuTemp );
      if( rotary.pressed()) {
        iMenu[MENU_YEAR] = MENUSTATE_WAIT_RELEASE;
      }
      break;
    case MENUSTATE_WAIT_RELEASE:
      if( !rotary.pressed()) { 
        iMenu[MENU_YEAR] = MENUSTATE_DONE;
        RTC.Get( td );
        iYear = rotary.position();
        lcd.setCursor( 0, 1 ); 
        lcd.print( "                     " );
        td.tm_year = iYear;
        RTC.Set( td );
      }
      break;
    case MENUSTATE_DONE:
        iMenu[MENU_YEAR] = MENUSTATE_SETUP;
        iActiveMenu = -1;
        iReturn = iYear;
      break;
    default:
      break;
  }
 return iReturn;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int MonthMenu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int MonthMenu () {
  int iReturn = -1;
  int iMonth ;
  switch( iMenu[MENU_MONTH] ){
    case MENUSTATE_SETUP:                             // rotery button released menu active
      rotary.minimum(1);
      rotary.maximum(12);
      RTC.Get( td );
      rotary.position( td.tm_mon +1 );                // tm_mon 0...11
      
      iMenu[MENU_MONTH] = MENUSTATE_WAIT_PRESS;       // we have set up the rotery switch 
      break;
    case MENUSTATE_WAIT_PRESS:                        // we have set up the  
      lcd.setCursor( 0, 1 ); 
      sprintf( szMenuTemp, "Month: %02d", rotary.position());
      lcd.print( szMenuTemp );
      if( rotary.pressed()) {
        iMenu[MENU_MONTH] = MENUSTATE_WAIT_RELEASE;
      }
      break;
    case MENUSTATE_WAIT_RELEASE:
      if( !rotary.pressed()) { 
        iMenu[MENU_MONTH] = MENUSTATE_DONE;
        RTC.Get( td );
        iMonth = rotary.position();
        lcd.setCursor( 0, 1 ); 
        lcd.print( "                     " );
        td.tm_mon = iMonth - 1 ;                      // tm_mon 0...11
        RTC.Set( td );
      }
      break;
    case MENUSTATE_DONE:
        iMenu[MENU_MONTH] = MENUSTATE_SETUP;
        iActiveMenu = -1;
        iReturn = iMonth;
      break;
    default:
      break;
  }
 return iReturn;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int DayMenu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int DayMenu () {
  int iReturn = -1;
  int iDay ;
  switch( iMenu[MENU_DAY] ){
    case MENUSTATE_SETUP:                             // rotery button released menu active
      rotary.minimum(1);
      rotary.maximum(31);
      RTC.Get( td );
      rotary.position( td.tm_mday );                  // tm_mon 1...31
      
      iMenu[MENU_DAY] = MENUSTATE_WAIT_PRESS;         // we have set up the rotery switch 
      break;
    case MENUSTATE_WAIT_PRESS:                        // we have set up the  
      lcd.setCursor( 0, 1 ); 
      sprintf( szMenuTemp, "Day: %02d", rotary.position());
      lcd.print( szMenuTemp );
      if( rotary.pressed()) {
        iMenu[MENU_DAY] = MENUSTATE_WAIT_RELEASE;
      }
      break;
    case MENUSTATE_WAIT_RELEASE:
      if( !rotary.pressed()) { 
        iMenu[MENU_DAY] = MENUSTATE_DONE;
        RTC.Get( td );
        iDay = rotary.position();
        lcd.setCursor( 0, 1 ); 
        lcd.print( "                     " );
        td.tm_mday = iDay ;                           // tm_mon 1...31
        RTC.Set( td );
      }
      break;
    case MENUSTATE_DONE:
        iMenu[MENU_DAY] = MENUSTATE_SETUP;
        iActiveMenu = -1;
        iReturn = iDay;
      break;
    default:
      break;
  }
 return iReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int HourMenu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int HourMenu () {
  int iReturn = -1;
  int iHour ;
  switch( iMenu[MENU_HOUR] ){
    case MENUSTATE_SETUP:                             // rotery button released menu active
      rotary.minimum(1);
      rotary.maximum(24);
      RTC.Get( td );
      rotary.position( td.tm_hour + 1 );
      iMenu[MENU_HOUR] = MENUSTATE_WAIT_PRESS;        // we have set up the rotery switch 
      break;
    case MENUSTATE_WAIT_PRESS:                        // we have set up the  
      lcd.setCursor( 0, 1 ); 
      sprintf( szMenuTemp, "Hour: %02d", rotary.position());
      lcd.print( szMenuTemp );
      if( rotary.pressed()) {
        iMenu[MENU_HOUR] = MENUSTATE_WAIT_RELEASE;
      }
      break;
    case MENUSTATE_WAIT_RELEASE:
      if( !rotary.pressed()) { 
        iMenu[MENU_HOUR] = MENUSTATE_DONE;
        RTC.Get( td );
        iHour = rotary.position();
        lcd.setCursor( 0, 1 ); 
        lcd.print( "                     " );
        td.tm_hour = iHour - 1 ;                      // tm_hour 0...23
        RTC.Set( td );
      }
      break;
    case MENUSTATE_DONE:
        iMenu[MENU_HOUR] = MENUSTATE_SETUP;
        iActiveMenu = -1;
        iReturn = iHour;
      break;
    default:
      break;
  }
 return iReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int MinuteMenu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int MinuteMenu () {
  int iReturn = -1;
  int iMinute ;
  switch( iMenu[MENU_MINUTE] ){
    case MENUSTATE_SETUP:                             // rotery button released menu active
      rotary.minimum(0);
      rotary.maximum(59);
      RTC.Get( td );
      rotary.position( td.tm_min  );
      iMenu[MENU_MINUTE] = MENUSTATE_WAIT_PRESS;      // we have set up the rotery switch 
      break;
    case MENUSTATE_WAIT_PRESS:                        // we have set up the  
      lcd.setCursor( 0, 1 ); 
      sprintf( szMenuTemp, "Minute: %02d", rotary.position());
      lcd.print( szMenuTemp );
      if( rotary.pressed()) {
        iMenu[MENU_MINUTE] = MENUSTATE_WAIT_RELEASE;
      }
      break;
    case MENUSTATE_WAIT_RELEASE:
      if( !rotary.pressed()) { 
        iMenu[MENU_MINUTE] = MENUSTATE_DONE;
        RTC.Get( td );
        iMinute = rotary.position();
        lcd.setCursor( 0, 1 ); 
        lcd.print( "                     " );
        td.tm_min = iMinute ;                         // tm_hour 0...59
        RTC.Set( td );
      }
      break;
    case MENUSTATE_DONE:
        iMenu[MENU_MINUTE] = MENUSTATE_SETUP;
        iActiveMenu = -1;
        iReturn = iMinute;
      break;
    default:
      break;
  }
 return iReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// int SecondMenu ()
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int SecondMenu () {
  int iReturn = -1;
  int iSecond ;
  switch( iMenu[MENU_SECOND] ){
    case MENUSTATE_SETUP:                             // rotery button released menu active
      rotary.minimum(0);
      rotary.maximum(59);
      RTC.Get( td );
      rotary.position( td.tm_sec  );
      iMenu[MENU_SECOND] = MENUSTATE_WAIT_PRESS;      // we have set up the rotery switch 
      break;
    case MENUSTATE_WAIT_PRESS:                        // we have set up the  
      lcd.setCursor( 0, 1 ); 
      sprintf( szMenuTemp, "Minute: %02d", rotary.position());
      lcd.print( szMenuTemp );
      if( rotary.pressed()) {
        iMenu[MENU_SECOND] = MENUSTATE_WAIT_RELEASE;
      }
      break;
    case MENUSTATE_WAIT_RELEASE:
      if( !rotary.pressed()) { 
        iMenu[MENU_SECOND] = MENUSTATE_DONE;
        RTC.Get( td );
        iSecond = rotary.position();
        lcd.setCursor( 0, 1 ); 
        lcd.print( "                     " );
        td.tm_sec = iSecond ;                         // tm_sec 0...59
        RTC.Set( td );
      }
      break;
    case MENUSTATE_DONE:
        iMenu[MENU_SECOND] = MENUSTATE_SETUP;
        iActiveMenu = -1;
        iReturn = iSecond;
      break;
    default:
      break;
  }
 return iReturn;
}


