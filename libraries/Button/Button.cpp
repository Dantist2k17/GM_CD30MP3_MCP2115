#include "Button.h"

Button::Button(byte pin, byte debounceTime, byte holdTime) {
  _pin          = pin;
  _debounceTime = debounceTime;
  _holdTime     = holdTime;
  pinMode(_pin, INPUT_PULLUP);
}

void Button::scanState() {
	currentTime = millis();
  	if(digitalRead(_pin) == LOW & !currentState) {
    	currentState = true;
    	previousTime = millis();
    	waitDebounce = true;
  	}
  	if((currentTime - previousTime) >= _debounceTime & waitDebounce & currentState) {
    	waitDebounce = false;
  	}
	if(!waitDebounce & currentState & digitalRead(_pin) == HIGH) {
    	currentState = false;
    	buttonError = false;
    	if((currentTime - previousTime) < (_holdTime*500)) {
      		buttonClick = true;
    	}
    	if(((currentTime - previousTime) >= (_holdTime*1000)) & ((currentTime - previousTime) < (2*_holdTime*1000))) {
      		buttonPress = true;
    	}
  	}
  	if(currentState & (currentTime - previousTime) >= (4*_holdTime*1000)) {
      	buttonError = true;
	}
}
