/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __UI_EVENT_TYPES_HPP__
#define __UI_EVENT_TYPES_HPP__


namespace Dodge {


typedef enum {
   UIEVENT_BTN1_PRESS,
   UIEVENT_BTN1_RELEASE,
   UIEVENT_BTN3_PRESS,
   UIEVENT_BTN3_RELEASE,
   UIEVENT_KEY_DOWN,
   UIEVENT_KEY_UP,
   UIEVENT_HOVER_ON,
   UIEVENT_HOVER_OFF
} uiEvent_t;


}


#endif
