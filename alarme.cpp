#include "alarme.h"


Alarme::Alarme(LiquidCrystal* lcd, AnalogButtons* Butnhandle, DS1302RTC* rtc) {
  pinMode(10, OUTPUT);
  this->_Butnhandle = Butnhandle;
  this->_lcd = lcd;
  this->_rtc = rtc;
  setSyncProvider(_rtc->get);
  this->Jour = new String[7] {"Dim ", "Lun ", "Mar ", "Mer ", "Jeu ", "Ven", "Sam "};
  this->_menuMsg = new String[3] {">Reg Date/Heure  ", ">Reg alarmes   ", ">Sortie    "};
  this->_curs = 0;
  this->Alrcount = 1;
  Button b1 = Button(1, 0  , 50 );
  Button b2 = Button(2, 490, 570);
  Button b3 = Button(3, 600, 750);
  _Butnhandle->addButton(b1);
  _Butnhandle->addButton(b2);
  _Butnhandle->addButton(b3);
  _lcd->begin(16, 2);
  _lcd->clear();
  _lcd->print("    School    ");
  _lcd->setCursor(0, 1);
  _lcd->print(" Alarme System ");
  breakTime(_rtc->get(), _time);
  _Smenu = none;
}
void Alarme::checkAlarme(){
   if( !second() ){
    for (byte k=1; k < 31; k++){ 
      byte HrA = EEPROM.read(k*3);
      byte MinA = EEPROM.read(k*3 + 1);
      byte ConfigA = EEPROM.read(k*3 + 2);
      if( hour() == HrA && minute() == MinA && bitRead(ConfigA, weekday())==1 ){
        digitalWrite(SONOR, 1);break; }
  }
  }
  if(second() > 5 && digitalRead(10)) digitalWrite(10, 0);
  
  
  }
void Alarme::menuPrincipale() {
  _lcd->clear();
  _lcd->print("Col Lala Meryem");
  _lcd->setCursor(0, 1);
Loop: int j = 16;
  while ( j > -18 ) {
    checkAlarme();
    _Butnhandle->checkButtons();
    if (_stateMenu )break;
    if (millis() - _currenttime > 200 ) {
      _currenttime = millis();
      /* */j--;
      _clearlcd();
      _lcd->setCursor(j, 1);
      breakTime(_rtc->get(), _time);
      _lcd->print( "<"+ String(Jour[_time.Wday - 1])+String(_time.Day) + "/" +
                  String(_time.Month) + "/" + String(tmYearToCalendar(_time.Year)) 
                  +" [" + String(_time.Hour) +
                  ":" + String(_time.Minute) +
                  "]" );
    
    }
    if (j < -15)goto Loop;
  }
  showText();

}
void Alarme::showText() {
  if (_stateMenu ) {

    _lcd->setCursor(0, 0);
    _lcd->print("----- Menu -----");
    _lcd->setCursor (0, 1);
    _clearlcd();
    _lcd->setCursor (0, 1);
    _lcd->print(_menuMsg[_item]);
  }
  else if (_Smenu == Stime) {
    _setTime();
  }
  else if (_Smenu == Salarme) {
    _setAlarme();
  }
  else {
    menuPrincipale();
  }

}
void Alarme::configUP() {
  if (_Smenu == Stime) {
    switch (_curs) {
      case 0:
        _time.Day += 1;
        if (_time.Day > 31) _time.Day = 1;
        _lcd->setCursor(CurCol[_curs] - 1, 0);
        _nbAff(_time.Day, "");
        _lcd->setCursor(CurCol[_curs], 0);
        break;
      case 1:
        _time.Month += 1;
        if (_time.Month > 12) _time.Month = 1;
        _lcd->setCursor(CurCol[_curs] - 1, 0);
        _nbAff(_time.Month, "");
        _lcd->setCursor(CurCol[_curs], 0);
        break;
      case 2:
        _time.Year += 1;
        _lcd->setCursor(CurCol[_curs] - 3, 0);
        _nbAff(_time.Year, "");
        _lcd->setCursor(CurCol[_curs], 0);
        break;
      case 3:
        _time.Hour += 1;
        if (_time.Hour > 23) _time.Hour = 0;
        _lcd->setCursor(CurCol[_curs] - 1, 1);
        _nbAff(_time.Hour, "");
        _lcd->setCursor(CurCol[_curs], 1);
      case 4:
        _time.Minute += 1;
        if (_time.Minute > 59) _time.Minute = 0;
        _lcd->setCursor(CurCol[_curs] - 1, 1);
        _nbAff(_time.Minute, "");
        _lcd->setCursor(CurCol[_curs], 1);
    }
  }
  if (_Smenu == Salarme)
  {
    switch (_curs) {
      case 0:
        Alrcount = (Alrcount > 15) ? 0 : Alrcount + 1;
        _HrAlr = EEPROM.read(Alrcount * 3);
        _MnAlr = EEPROM.read(Alrcount * 3 + 1);
        _configAlr = EEPROM.read(Alrcount * 3 + 2);
        _lcd->setCursor(0, 1);
        _nbAff(Alrcount, " ");
        _nbAff((_HrAlr > 23) ? 0 : _HrAlr, ":");
        _nbAff( (_MnAlr > 59) ? 0 : _MnAlr, " ");
        for (byte j = 1; j <= 7; j++) _lcd->print(bitRead(_configAlr, j));
        _lcd->setCursor(CurColAlarm[_curs], 1);
        break;
      case 1:
        _HrAlr+=1;
        _lcd->setCursor(CurColAlarm[_curs] - 1, 1);
        _nbAff(_HrAlr=(_HrAlr > 23) ? 0 : _HrAlr, "");
        _lcd->setCursor(CurColAlarm[_curs], 1);
        break;
      case 2:
        _MnAlr+=1;
        _lcd->setCursor(CurColAlarm[_curs] - 1, 1);
        _nbAff( _MnAlr=(_MnAlr > 59) ? 0 : _MnAlr, "");
        _lcd->setCursor(CurColAlarm[_curs], 1);
        break;
      default:
        bitWrite(_configAlr, _curs - 2, !bitRead(_configAlr, _curs - 2));
        _lcd->setCursor(CurColAlarm[_curs] , 1);
        _lcd->print(bitRead(_configAlr, _curs - 2));
        _lcd->setCursor(CurColAlarm[_curs] , 1);
        
        break;
    }
  }
}
void Alarme::configDOWN() {
  if (_Smenu == Stime) {
    switch (_curs) {
      case 0:
        _time.Day -= 1;
        if (_time.Day < 1) _time.Day = 31;
        _lcd->setCursor(CurCol[_curs] - 1, 0);
        _nbAff(_time.Day, "");
        break;
      case 1:
        _time.Month -= 1;
        if (_time.Month < 1) _time.Month = 12;
        _lcd->setCursor(CurCol[_curs] - 1, 0);
        _nbAff(_time.Month, "");
        _lcd->setCursor(CurCol[_curs], 0);
        break;
      case 2:
        _time.Year -= 1;
        if (_time.Year < 45) _time.Year = 45;
        _lcd->setCursor(CurCol[_curs] - 3, 0);
        _nbAff(_time.Year, "");
        _lcd->setCursor(CurCol[_curs], 0);
        break;
      case 3:
        _time.Hour -= 1;
        if (_time.Hour < 0) _time.Hour = 23;
        _lcd->setCursor(CurCol[_curs] - 1, 1);
        _nbAff(_time.Hour, "");
        _lcd->setCursor(CurCol[_curs], 1);
      case 4:
        _time.Minute -= 1;
        if (_time.Minute < 0) _time.Minute = 59;
        _lcd->setCursor(CurCol[_curs] - 1, 1);
        _nbAff(_time.Minute, "");
        _lcd->setCursor(CurCol[_curs], 1);
    }
  }
  if (_Smenu == Salarme)
  {
    switch (_curs) {

      case 0:
        Alrcount-=1;
        if (Alrcount ==0) {
          _clearlcd();
          _Smenu=none;
          _stateMenu=1;
          _item=2;
          _lcd->setCursor(2, 1);
          _lcd->print("<Save>");
          Save();
         Alrcount=1;
          break;
        }

        _HrAlr = EEPROM.read(Alrcount * 3);
        _MnAlr = EEPROM.read(Alrcount * 3 + 1);
        _configAlr = EEPROM.read(Alrcount * 3 + 2);
        _lcd->setCursor(0, 1);
        _nbAff(Alrcount, " ");
        _nbAff(_HrAlr=(_HrAlr > 23) ? 0 : _HrAlr, ":");
        _nbAff( _MnAlr=(_MnAlr > 59) ? 0 : _MnAlr, " ");
        for (byte j = 1; j <= 7; j++) _lcd->print(bitRead(_configAlr, j));
        _lcd->setCursor(CurColAlarm[_curs], 1);
        break;
      case 1:
        _HrAlr-=1;
        _lcd->setCursor(CurColAlarm[_curs] - 1, 1);
        _nbAff(_HrAlr=(_HrAlr >23 ) ? 23 : _HrAlr, "");
        _lcd->setCursor(CurColAlarm[_curs], 1);
        break;
      case 2:
        _MnAlr-=1;
        _lcd->setCursor(CurColAlarm[_curs] - 1, 1);
        _nbAff(_MnAlr=(_MnAlr >59) ? 59 : _MnAlr, "");
        _lcd->setCursor(CurColAlarm[_curs], 1);
        break;
      default:
        bitWrite(_configAlr, _curs - 2, !bitRead(_configAlr, _curs - 2));
        _lcd->setCursor(CurColAlarm[_curs] , 1);
        _lcd->print(bitRead(_configAlr, _curs - 2));
        _lcd->setCursor(CurColAlarm[_curs] , 1);
        
        break;
    }
  }
}
void Alarme::Save() {
  EEPROM.update(Alrcount * 3, _HrAlr);
  EEPROM.update(Alrcount * 3 + 1, _MnAlr);
  EEPROM.update(Alrcount * 3 + 2, _configAlr);
}
void Alarme::_nbAff(int nbr, String chaine) {
  if (nbr < 10) _lcd->print('0');
  _lcd->print(String(nbr) + chaine);
}
void Alarme::_setTime() {
  // breakTime(_rtc->get(), _time);
  _lcd->setCursor(0, 0);
  _lcd->print("    ");
  _nbAff(_time.Day, "/");
  _nbAff(_time.Month, "/");
  _nbAff(tmYearToCalendar(_time.Year), "   ");
  _lcd->setCursor(0, 1);
  _lcd->print("  ");
  _nbAff(_time.Hour, ":");
  _nbAff(_time.Minute, ":");
  _nbAff(_time.Second, "  <ok>");
  if (_curs > 5) {
    _rtc->write(_time);
    setSyncProvider(_rtc->get);
    _lcd->noBlink();
    _Smenu = none;
    _stateMenu = 0;
    _curs = 0;
    menuPrincipale();

  }
  (_curs < 3) ? _lcd->setCursor(CurCol[_curs], 0) : _lcd->setCursor(CurCol[_curs], 1);
  _lcd->blink();

}
void Alarme::_setAlarme() {
  _lcd->setCursor(0, 0);
  _lcd->print("AL Heure DLMMJVS");
  _HrAlr = EEPROM.read(Alrcount * 3);
  _MnAlr = EEPROM.read(Alrcount * 3 + 1);
  _configAlr = EEPROM.read(Alrcount * 3 + 2);
  _lcd->setCursor(0, 1);
  _nbAff(Alrcount, " ");
  _nbAff((_HrAlr > 23) ? 0 : _HrAlr, ":");
  _nbAff( (_MnAlr > 59) ? 0 : _MnAlr, " ");
  for (byte j = 1; j <= 7; j++) _lcd->print(bitRead(_configAlr, j));
  _lcd->setCursor(CurColAlarm[_curs], 1);
  _lcd->blink();

}
void Alarme::_clearlcd() {
  for (uint8_t k = 0; k <= 15; k++) {
    _lcd->print(" ");
  }
}
