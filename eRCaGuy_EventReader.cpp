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
const int8_t eRCaGuy_EventReader::NO_ACTION = 0;
const int8_t eRCaGuy_EventReader::ACTION_OCCURRED = 1;
const int8_t eRCaGuy_EventReader::ACTION_UNOCCURRED = -1;

//define class constructor method
eRCaGuy_EventReader::eRCaGuy_EventReader(unsigned int debounceDelay, bool eventStateWhenEventOccurs)
{
  //initialize _EVENT_OCCURRING and _EVENT_NOT_OCCURRING member variables
  setEventStateWhenEventOccurs(eventStateWhenEventOccurs);
	
  //initialize member variables
  _debounceDelay = debounceDelay;
	//for readEvent method
	_lastBounceTime = 0; //ms; the last time the event state bounced (ie: the time stamp when the eventState last changed)
	_eventStateOld = !eventStateWhenEventOccurs; //the previous event state; initialize as though no event was occurring last time 
	_debouncedState = _EVENT_NOT_OCCURRING; //the current, actual, NOT bouncing event state; initialize as _EVENT_NOT_OCCURRING
	_debouncedStateOld = _EVENT_NOT_OCCURRING; //the previous, actual, NOT bouncing event state; initialize as _EVENT_NOT_OCCURRING, so that we start out looking for an ACTION_OCCURRED, NOT an ACTION_UNOCCURRED
}

//define other class methods (functions)

//------------------------------------------------------------------------------------------------------
//setDebounceDelay
//------------------------------------------------------------------------------------------------------
void eRCaGuy_EventReader::setDebounceDelay(unsigned int debounceDelay)
{
  _debounceDelay = debounceDelay; //ms; the minimum time that the value of the event must *not* change in order to be considered the new, *true*, NOT bouncing state of the event
}

//------------------------------------------------------------------------------------------------------
//setEventStateWhenEventOccurs(0 or 1)
//-This specifies whether the event state will be 0 or 1 when the event is occurring
//--ex: if using a pull-up resistor on a button or switch, the event state will be LOW when the button is pressed
//--if using a pull-down resistor on a button or switch, the event state will be HIGH when the button is pressed
//------------------------------------------------------------------------------------------------------
void eRCaGuy_EventReader::setEventStateWhenEventOccurs(bool eventStateWhenEventOccurs)
{
  _EVENT_OCCURRING = eventStateWhenEventOccurs;
  _EVENT_NOT_OCCURRING = !eventStateWhenEventOccurs;
}

//------------------------------------------------------------------------------------------------------
//getDebounceDelay
//------------------------------------------------------------------------------------------------------
unsigned int eRCaGuy_EventReader::getDebounceDelay()
{
  return _debounceDelay; //ms
}

//------------------------------------------------------------------------------------------------------
//getEventStateWhenEventOccurs
//------------------------------------------------------------------------------------------------------
bool eRCaGuy_EventReader::getEventStateWhenEventOccurs()
{
  return _EVENT_OCCURRING; 
}

//------------------------------------------------------------------------------------------------------
//readEvent
/*
Read the event action, and store it into the debouncedAction variable; and read the event state, & store it into the debouncedState variable
-The event state can be 0 or 1
-event action indicates what just happened to the event: 
  0 = NO_ACTION: no change in true, debounced event state since last time interpreting the event, or debounceDelay time not yet elapsed <--*perhaps* in the future, output a 3 to indicate debounceDelay time not yet elapsed
  1 = ACTION_OCCURRED: a new event just occurred (debounceDelay had elapsed)
 -1 = ACTION_UNOCCURRED: event just "un-occurred" by going back to its resting state (debounceDelay had elapsed)
*/
//------------------------------------------------------------------------------------------------------
void eRCaGuy_EventReader::readEvent(bool eventState, int8_t *debouncedAction, boolean* debouncedState)
{
  int8_t action = NO_ACTION;
  
  //0) Update _lastBounceTime each time a bounce occurs
  if (eventState != _eventStateOld) //check to see if the *bouncing* event state has changed
  {
    _eventStateOld = eventState; //update
    _lastBounceTime = millis(); //ms; update 
  }
  //1) Only conclude that the eventState is at its actual (not bouncing) state if the debounceDelay has elapsed, as this means that the event state is now resting in its final, NOT bouncing condition.
  if (millis() - _lastBounceTime > _debounceDelay) //if the debounce time has elapsed
  {
    //since the debounceDelay has elapsed, this means the eventState is no longer changing, which means bouncing is no longer occurring,
    //which means that the current eventState is the actual, new, NOT bouncing event state
    _debouncedState = eventState; //store the actual, NOT bouncing even state
    //2) Check to see if the *actual, NOT bouncing* event state has CHANGED
    if (_debouncedState != _debouncedStateOld) //if the actual, NOT bouncing event state has changed
    {
      //3) Since we know that the event is real (debounced), and not noise (since the debounceDelay has occurred), and since we know the event state has CHANGED, meaning that the event isn't just sitting constant in an _EVENT_OCCURRING or _EVENT_NOT_OCCURRING *state*, let's check to see if the event action is ACTION_OCCURRED or ACTION_UNOCCURRED, then the user can act on the side of the action that he/she sees fit
      if (_debouncedState==_EVENT_OCCURRING)
      {
        action = ACTION_OCCURRED; //event action just took place (and debounceDelay had elapsed)
      }
      else //_debouncedState==_EVENT_NOT_OCCURRING
      {
        action = ACTION_UNOCCURRED; //event just went back to its normal, resting condition (and debounceDelay had elapsed)
      }
    } //end of checking to see if the button state has CHANGED
    _debouncedStateOld = _debouncedState; //update the old button state
  } //end of Button debouncing
  
  //update the output variables, via the pointers passed in to the function
  *debouncedAction = action;
  *debouncedState = _debouncedState;
}

