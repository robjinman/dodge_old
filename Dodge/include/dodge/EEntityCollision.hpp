/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __E_ENTITY_COLLISION_HPP__
#define __E_ENTITY_COLLISION_HPP__


#include "StringId.hpp"
#include "EEvent.hpp"
#include <boost/shared_ptr.hpp>


namespace Dodge {


class Entity;

class EEntityCollision : public EEvent {
   public:
      EEntityCollision(bool b, boost::shared_ptr<Entity> A, boost::shared_ptr<Entity> B)
         : EEvent(internString("entityCollision")), incoming(b), entityA(A), entityB(B) {}

      bool incoming;
      boost::shared_ptr<Entity> entityA;
      boost::shared_ptr<Entity> entityB;
};


}


#endif
