#ifndef alarme_h
#define alarme_h
#include "Arduino.h"
#include <LiquidCrystal.h>
#include <AnalogButtons.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>
#include <Wire.h>
#include <EEPROM.h>
#define SONOR  10
//#include <LiquidCrystal_I2C.h>
//lcd(rs, en, d4, d5, d6, d7);----++++++++


class Alarme {
  private:
    LiquidCrystal* _lcd;
    AnalogButtons* _Butnhandle;
    DS1302RTC* _rtc;
    unsigned long _currenttime = 0;
    tmElements_t _time;
    String* Jour;
    byte _configAlr = 0;
    byte _HrAlr = 0, _MnAlr = 0;
    void _setTime();
    byte Alrcount;
    void _setAlarme();
    void _clearlcd();
    void menuPrincipale();
    void _nbAff(int nbr, String chaine);
    byte CurCol[6] = {5, 8, 13, 3, 6, 12};
    byte CurColAlarm[10] = {1, 4, 7, 9, 10, 11, 12, 13, 14, 15};
  public:
    Alarme(LiquidCrystal* lcd, AnalogButtons* Butnhandle, DS1302RTC* rtc);
    void showText();
    void configUP();
    void configDOWN();
    void Save();
    void checkAlarme();
    String* _menuMsg;
    boolean _stateMenu ;
    uint8_t _item;
    byte _curs;
    enum _sM {none, Stime, Salarme, Sexit, CursTime, curFal} _Smenu;
};
#endif
