#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button {
  public:
    Button(byte pin, byte debounceTime, byte holdTime);
    
    bool buttonClick = false;	//признак кратковременного нажатия (< 0.5*holdtime)
    bool buttonPress = false;  	//признак длительного нажатия      (>= holdtime)
    bool buttonError = false;  	//признак залипания кнопки         (>= 4*holdtime)
    
    void scanState();

  private:
    byte    _pin;               //номер вывода
    byte    _debounceTime;      //время подавления дребезга в миллисекундах
    byte    _holdTime;          //время удержания кнопки в секундах, при котором нажатие считается продолжительным

    unsigned long currentTime = 0;  
    unsigned long previousTime = 0;
    bool currentState = false;
    bool waitDebounce = false;
};
#endif