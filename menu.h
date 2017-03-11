//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// menus.h
//
// Date:      2017/03/9
// By:        Suusi Malcolm-Brown
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MENU_H
  #define MENU_H

enum MENUSTATE_t {
                  MENUSTATE_SETUP = 0,
                  MENUSTATE_WAIT_PRESS,
                  MENUSTATE_WAIT_RELEASE,
                  MENUSTATE_INITIALRELEASE,
                  MENUSTATE_WAIT_RELEASE1,
                  MENUSTATE_DONE
  };

enum MENUS_t {
                  MENU_MAIN = 0,
                  MENU_YEAR,
                  MENU_MONTH,
                  MENU_DAY,
                  MENU_HOUR,
                  MENU_MINUTE,
                  MENU_SECOND,
                  MENU_DOW
  };

void Menu();
int  MainMenu ();
int  YearMenu ();
int  MonthMenu ();
int  DayMenu ();
int  HourMenu ();
int  MinuteMenu ();
int  SecondMenu ();

#endif
