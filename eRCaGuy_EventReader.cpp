/*
eRCaGuy_EventReader
-see .h file for details 

References:


*/

/*
===================================================================================================
  LICENSE & DISCLAIMER
  Copyright (C) 2014-2016 Gabriel Staples.  All right reserved.
  
  This file is part of eRCaGuy_EventReader.
  
  I AM WILLING TO DUAL-LICENSE THIS SOFTWARE. HOWEVER, UNLESS YOU HAVE PAID FOR AND RECEIVED A RECEIPT
  FOR AN ALTERNATE LICENSE AGREEMENT, FROM ME, THE COPYRIGHT OWNER, THIS SOFTWARE IS LICENSED AS FOLLOWS: 
  
  ------------------------------------------------------------------------------------------------
  License: GNU General Public License Version 3 (GPLv3) - https://www.gnu.org/licenses/gpl.html
  ------------------------------------------------------------------------------------------------
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/
===================================================================================================
*/

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include "eRCaGuy_EventReader.h"

//define class constants
const int8_t eRCaGuy_EventReader::PRESSED_ACTION = 1;
const int8_t eRCaGuy_EventReader::RELEASED_ACTION = -1;

//define class constructor method
eRCaGuy_EventReader::eRCaGuy_EventReader(uint8_t buttonPin,unsigned int debounceDelay,boolean pinStateWhenButtonPressed)
{
  //initialize _BUTTON_PRESSED and _BUTTON_NOT_PRESSED member variables
  setPinStateWhenButtonPressed(pinStateWhenButtonPressed);
	
  //initialize member variables
  _buttonPin = buttonPin;
  _debounceDelay = debounceDelay;
	//for readButton method
	_lastBounceTime = 0; //ms; the last time the button bounced (ie: the time stamp when the digital reading value last changed)
	_reading_old = digitalRead(_buttonPin); //the previous reading; initialize as the first reading when instantiating an object
	_buttonState = _BUTTON_NOT_PRESSED; //the current, actual, NOT bouncing button state; initialize as BUTTON_NOT_PRESSED
	_buttonState_old = _BUTTON_NOT_PRESSED; //the previous, actual, NOT bouncing button state; initialize as BUTTON_NOT_PRESSED, so that we start out looking for a button press, NOT a button release
}

//define other class methods (functions)

//------------------------------------------------------------------------------------------------------
//setDebounceDelay
//------------------------------------------------------------------------------------------------------
void eRCaGuy_EventReader::setDebounceDelay(unsigned int debounceDelay)
{
  _debounceDelay = debounceDelay; //ms; the minimum time that the value of the button (or switch) must *not* change in order to be considered the new, *true*, NOT bouncing state of the button
}

//------------------------------------------------------------------------------------------------------
//setButtonPin
//------------------------------------------------------------------------------------------------------
void eRCaGuy_EventReader::setButtonPin(uint8_t buttonPin)
{
  _buttonPin = buttonPin;
}

//------------------------------------------------------------------------------------------------------
//setPinStateWhenButtonPressed(HIGH or LOW)
//-This specifies whether the pin state of the buttonPin will be HIGH or LOW when the button is pressed
//--if using a pull-up resistor on the button or switch, the pin state will be LOW when the button is pressed
//--if using a pull-down resistor on the button or switch, the pin state will be HIGH when the button is pressed
//------------------------------------------------------------------------------------------------------
void eRCaGuy_EventReader::setPinStateWhenButtonPressed(boolean pinStateWhenButtonPressed)
{
  if (pinStateWhenButtonPressed==LOW)
  {
    //the following is true when using a pull-down resistor on the button
	_BUTTON_PRESSED = LOW;
	_BUTTON_NOT_PRESSED = HIGH;
  }
  else //pinStateWhenButtonPressed==HIGH
  {
    //the following is true when using a pull-up resistor on the button
	_BUTTON_PRESSED = HIGH;
	_BUTTON_NOT_PRESSED = LOW;
  }
}

//------------------------------------------------------------------------------------------------------
//getDebounceDelay
//------------------------------------------------------------------------------------------------------
unsigned int eRCaGuy_EventReader::getDebounceDelay()
{
  return _debounceDelay; //ms; return the _debounceDelay variable in ms
}

//------------------------------------------------------------------------------------------------------
//readButton
//------------------------------------------------------------------------------------------------------
//read the button action, and store it into the button_action input variable; and read the button state, & store it into the button_state input variable
//The button state can be 0 or 1, for LOW or HIGH, respectively
//Button action indicates what just happened to the button: 
//	0 = no-change in true, debounced button state, or debounceDelay time not yet elapsed <--*perhaps* in the future, output a 3 to indicate 
//		debounceDelay time not yet elapsed
//	1 = button was just *pressed* by a human operator (debounceDelay had elapsed)
// -1 = button was just *released* by a human operator (debounceDelay had elapsed)
void eRCaGuy_EventReader::readButton(int8_t* button_action, boolean* button_state)
{
  int8_t buttonAction = 0; //indicates what just happened to the button: 0 = no-change in button state, or debounceDelay time not yet elapsed <--*perhaps* in the future, output a 3 to indicate debounceDelay time not yet elapsed
                           //                                            1 = button was just pressed by a human operator (debounceDelay had elapsed)
                           //                                           -1 = button was just released by a human operator (debounceDelay had elapsed)
  //0) Update _lastBounceTime each time a bounce occurs
  unsigned int reading = digitalRead(_buttonPin); //get a new reading
  if (reading != _reading_old) //check to see if the *bouncing* button state has changed
  {
    _reading_old = reading; //update
    _lastBounceTime = millis();
  }
  //1) Only conclude that the button is at its actual (not bouncing) button state if the debounceDelay has elapsed, as this means
  //   that the button is now resting in its final, NOT bouncing condition.
  if (millis() - _lastBounceTime > _debounceDelay) //if the debounce time has elapsed
  {
    //since the debounceDelay has elapsed, this means the reading is no longer changing, which means bouncing is no longer occurring,
    //which means that the current reading is the actual, new, NOT bouncing button state
    _buttonState = reading; //store the actual, NOT bouncing button state
    //2) Check to see if the *actual, NOT bouncing* button state has CHANGED
    if (_buttonState != _buttonState_old) //if the actual, NOT bouncing button state has changed
    {
      //3) Since we know that the press was by a human, and not noise (since the debounceDelay has occurred), and since we know the button state has
      //   CHANGED, meaning that the button isn't just sitting constant in a pressed or released state, let's check to see if the button is PRESSED,
      //   or UN-PRESSED/RELEASED, then act on the side of the action that we see fit
      if (_buttonState==_BUTTON_PRESSED) //if a button press is detected
      {
        buttonAction = 1; //button just pressed by a human operator (debounceDelay had elapsed)
      }
      else //_buttonState==_BUTTON_NOT_PRESSED
      {
        buttonAction = -1; //button was just released by a human operator (debounceDelay had elapsed)
      }
    } //end of checking to see if the button state has CHANGED
    _buttonState_old = _buttonState; //update the old button state
  } //end of Button debouncing & Output Toggle code
  
  //update the output variables, via the pointers passed in to the function
  *button_action = buttonAction;
  *button_state = _buttonState;
}

