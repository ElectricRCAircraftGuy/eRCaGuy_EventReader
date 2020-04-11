/*
Gabriel Staples
www.ElectricRCAircraftGuy.com

10 Apr. 2020 

Posted here: https://stackoverflow.com/questions/60850806/how-to-debounce-a-bitmask-readings-with-arduino-not-buttons/61153060#61153060

*/

#include <eRCaGuy_EventReader.h>

// 1500ms, or 1.5 sec debounce delay (last state must remain unchanged for this duration or else the
// debounce is not yet considered "complete")
constexpr unsigned int DEBOUNCE_DELAY_MS = 1500; 
constexpr bool EVENT_STATE_WHEN_EVENT_OCCURS = true;

eRCaGuy_EventReader debouncer(DEBOUNCE_DELAY_MS, EVENT_STATE_WHEN_EVENT_OCCURS);

void setup()
{

}

void loop()
{
  // Check your masks here
  // ***FILL IN YOUR REAL LOGIC HERE TO CHECK IF THE MASKS ARE THE SAME!***
  bool masks_are_same = true; 

  // Register a new reading
  debouncer.readEvent(masks_are_same);

  // Now get debounced values

  int8_t debounced_action = debouncer.getDebouncedAction();
  switch (debounced_action)
  {
    // No change in the true, debounced event state since last time interpreting the event, or 
    // debounceDelay time not yet elapsed
    case eRCaGuy_EventReader::NO_ACTION:
      // do what you want here
      break;
    // A new event just occurred (debounce delay has elapsed)
    // ie: debounced version of `masks_are_same` has gone from false to true!
    case eRCaGuy_EventReader::ACTION_OCCURRED:
      // do what you want here
      break;
    // A new event just "un-occurred" by going back to its resting state (debounce delay has 
    // elapsed); ie: debounced version of `masks_are_same` has just gone from true back to false!
    case eRCaGuy_EventReader::ACTION_UNOCCURRED:
      // do what you want here
      break;
  }

  // This is the debounced version of `masks_are_same`
  bool masks_are_same_debounced = debouncer.getDebouncedState();
  if (masks_are_same_debounced)
  {
    // do whatever you want if the masks are the same
  }
  else
  {
    // do whatever you want if the masks are different
  }
}