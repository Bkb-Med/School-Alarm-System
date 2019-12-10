

 /*                                              *****************************************************
  *                                              *                School Alarm System                * 
                                                 * @author : Med Bkb  10/12/19 08:22                 *
                                                 * Original code was forked from Med EZZINE Code     *
                                                 * Under MIT licence                                 * 
                                                 *****************************************************
 */
#include <LiquidCrystal.h>
#include <AnalogButtons.h>
#include <DS1302RTC.h>
#include "alarme.h"

LiquidCrystal lcd(9, 11, 5, 4, 3, 2);
AnalogButtons handlebutton(A0, 100, &handlebut);
DS1302RTC RTC(A3, A4, A5);//DS1302RTC(uint8_t CE_pin, uint8_t SCLK_pin, uint8_t IO_pin);
Alarme _alr(&lcd, &handlebutton, &RTC);

void handlebut(int id, boolean longAppui) {
  switch (id) {
    case 1://-
      if ( _alr._stateMenu) {
        _alr._item = ( _alr._item <= 0) ? 0 : _alr._item -= 1;
        _alr.showText();break;
      }
       if(_alr._Smenu==_alr.Stime || _alr._Smenu==_alr.Salarme ){
        _alr.configUP();
        
      }

      
      break;
    case 2://+
      if ( _alr._stateMenu) {
        _alr._item = ( _alr._item >= 2) ? 2 :  _alr._item += 1;
        _alr.showText();break;
      }
      if(_alr._Smenu==_alr.Stime || _alr._Smenu==_alr.Salarme){
        _alr.configDOWN();
      }
      break;
    case 3://ok//menu  
              if( _alr._Smenu==_alr.Stime){
             _alr._curs=(_alr._curs>5)?0:_alr._curs+1;_alr.showText(); break;}
              if( _alr._Smenu==_alr.Salarme){
             _alr._curs=(_alr._curs>9)?0:_alr._curs+1;_alr.Save();_alr.showText(); break;}
             _alr._Smenu=(_alr._item == 0 && _alr._stateMenu==1) ? _alr.Stime : (_alr._item == 1 && _alr._stateMenu==1)?_alr.Salarme:_alr.Sexit;        
             _alr._item = 0; 
             _alr._stateMenu = !_alr._stateMenu ; 
             _alr.showText();
             break;
  }
}

void setup() {

}

void loop() {

  handlebutton.checkButtons();
   _alr.checkAlarme();

}
