#ifndef __BOX2D_CONTACT_LISTENER_HPP__
#define __BOX2D_CONTACT_LISTENER_HPP__


#include "../Box2D/Box2D.h"
#include "EventManager.hpp"


namespace Dodge {


class Box2dContactListener : public b2ContactListener {
   public:
      Box2dContactListener()
         : b2ContactListener() {}

      virtual ~Box2dContactListener() {}

   private:
      virtual void BeginContact(b2Contact* contact);
      virtual void EndContact(b2Contact* contact);

      EventManager m_eventManager;
};


}


#endif
