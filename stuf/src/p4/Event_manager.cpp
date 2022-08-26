// updated Mon Nov  9 22:38:22 PST 2020

#include <memory>
#include <cassert>
#include "Event_manager.h"


/* static */ Event_manager&  Event_manager::instance()
{
  static Event_manager the_manager;
  return the_manager;
}

Event_manager::~Event_manager()
{
  #ifdef P5
    assert(false && "Implement the Event_manager destructor for p5!");
  #endif
}

void Event_manager::execute_handlers(Window::Keystroke keystroke) const
{
  #ifdef P5
    assert(false && "Implement Event_manager::execute_handlers for p5!");
  #endif
}
