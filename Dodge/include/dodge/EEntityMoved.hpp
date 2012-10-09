/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __E_ENTITY_MOVED_HPP__
#define __E_ENTITY_MOVED_HPP__


#include "StringId.hpp"
#include "EEvent.hpp"
#include "Range.hpp"
#include <boost/shared_ptr.hpp>


namespace Dodge {


class Entity;

class EEntityMoved : public EEvent {
   public:
      EEntityMoved(boost::shared_ptr<Entity> ent, const Range& oldBounds, const Range& newBounds)
         : EEvent(internString("entityMoved")), entity(ent), oldBoundingBox(oldBounds), newBoundingBox(newBounds) {}

      boost::shared_ptr<Entity> entity;
      Range oldBoundingBox;
      Range newBoundingBox;
};


}


#endif
