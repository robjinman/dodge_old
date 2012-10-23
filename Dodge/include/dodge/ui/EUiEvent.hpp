/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __E_UI_EVENT_HPP__
#define __E_UI_EVENT_HPP__


#include <boost/shared_ptr.hpp>
#include "../EEvent.hpp"
#include "../StringId.hpp"
#include "uiEventTypes.hpp"


namespace Dodge {


class Entity;

class EUiEvent : public EEvent {
   public:
      EUiEvent(uiEvent_t type, boost::shared_ptr<Entity> ent)
         : EEvent(internString("uiEvent")), eventType(type), entity(ent) {}

      uiEvent_t eventType;
      boost::shared_ptr<Entity> entity;
};


}


#endif
