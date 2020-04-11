/*
eRCaGuy_EventReader
-a continuation of eRCaGuy_ButtonReader

ButtonReader Library webpage: http://www.electricrcaircraftguy.com/2014/05/ercaguybuttonreader-library-for-arduino.html
-I wanted a simple and universal, yet very powerful & reliable library to read a button or switch in such a way that I can
 easily implement toggled actions and momentary actions, know the true, debounced state of a button or switch at any given time, 
 and specify whether I want an action to take place when the button is *pressed* or *released.*  This library makes implementing all of
 these things easy for me, and I now use it on all of my projects that require reading a button or switch, so I wanted to share it with others.  Hopefully you find it useful too.
 
 By Gabriel Staples
 http://www.ElectricRCAircraftGuy.com
 -My contact info is available by clicking the "Contact Me" tab at the top of my blog.
 ButtonReader Written: 30 May 2014
 ButtonReader Last Updated: 31 Oct 2014
 EventReader Started: 13 Dec 2016 
 EventReader Last Updated: 13 Dec 2016 
 
 Version History (newest on top):
  - 20161213 - v2.1.0 released; I am now storing _debouncedAction and _debouncedState right inside the objects! getDebouncedAction() and getDebouncedState() method added. It is no longer necessary to keep track of these outside of the library anymore!
  - 20161213 - v2.0.0 released (first version now called eRCaGuy_EventReader instead of eRCaGuy_ButtonReader); this is a major overhaul and upgrade! This version is now capable of debouncing *any* event instead of just digitalRead inputs on buttons. This means that you can interpret and debounce capacitive touch buttons, or ladders of buttons read as different voltage values on a single analog pin, for instance. This is now possible because instead of letting the library read the button via digitalRead, you just pass in the current button or event state as a 0 or 1 to the library, via the "readEvent" method, and let it interpret and debounce it for you. 
  - 20141031 - v1.1 release; fixed major bug in eRCaGuy_ButtonReader which prevented reading multiple buttons simultaneously from working
  - 20140531 - v1.0 release; first release of eRCaGuy_ButtonReader
 
 Credits:
 1) This file was created and edited in Notepad++ (http://notepad-plus-plus.org/)
 2) The core of my debouncing algorithm is based on the Arduino "Debounce" example (which is a very thorough and well-written debounce
    by the way) here:  http://arduino.cc/en/Tutorial/Debounce
    -The history of the above example, in order to properly thank and give credit to these people, is:
      -created 21 November 2006 by David A. Mellis
      -modified 30 Aug 2011 by Limor Fried (LadyAda)
      -modified 28 Dec 2012 by Mike Walters
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

#ifndef eRCaGuy_EventReader_h
#define eRCaGuy_EventReader_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

class eRCaGuy_EventReader
{
  public:
    //declare class constructor method
    eRCaGuy_EventReader(unsigned int debounceDelay = 50, bool eventStateWhenEventOccurs = 1); //default debounceDelay, if not specified, is this value here 
  
    //set functions 
    
    //set the debounceDelay time (in ms)
    void setDebounceDelay(unsigned int debounceDelay = 50); //default is 50ms
    //set whether the event is considered to be occurring when the state is HIGH (1) or LOW (0)
    void setEventStateWhenEventOccurs(bool eventStateWhenEventOccurs = 1);
    
    //get functions 
    
    //get the current debounceDelay value 
    unsigned int getDebounceDelay();
    bool getEventStateWhenEventOccurs();
    
    /*
    Read the latest eventState, debounce it, and update _debouncedAction and _debouncedState as necessary
    -The event state can be 0 or 1
    -_debouncedAction indicates what just happened to the event: 
      0 = NO_ACTION: no change in true, debounced event state since last time interpreting the event, or debounceDelay time not yet elapsed <--*perhaps* in the future, output a 3 to indicate debounceDelay time not yet elapsed
      1 = ACTION_OCCURRED: a new event just occurred (debounceDelay had elapsed)
     -1 = ACTION_UNOCCURRED: event just "un-occurred" by going back to its resting state (debounceDelay had elapsed)
    */
    void readEvent(bool eventState);
    
    //get debounced values 
    int8_t getDebouncedAction();
    bool getDebouncedState();
    
    //Public class constants to define ACTIONS
    //define class constants
    static constexpr int8_t NO_ACTION = 0;
    static constexpr int8_t ACTION_OCCURRED = 1;
    static constexpr int8_t ACTION_UNOCCURRED = -1;

  private:
    //declare private class methods (functions)
    //N/A
    //variables accessible by this class only
    unsigned int _debounceDelay;
    //Event states:
    bool _EVENT_OCCURRING;
    bool _EVENT_NOT_OCCURRING;
    //for readEvent method
    unsigned long _lastBounceTime;
    unsigned int _eventStateOld;
    int8_t _debouncedAction;
    boolean _debouncedState;
    boolean _debouncedStateOld;
};

#endif
