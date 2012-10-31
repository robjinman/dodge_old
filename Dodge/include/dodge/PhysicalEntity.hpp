/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PHYSICAL_ENTITY_HPP__
#define __PHYSICAL_ENTITY_HPP__


#include <boost/shared_ptr.hpp>
#include "Entity.hpp"
#include "EntityPhysics.hpp"


namespace Dodge {


class PhysicalEntity : public Entity, public EntityPhysics {
   public:
      PhysicalEntity(pEntityPhysicsImpl_t impl, long type)
         : Entity(type), EntityPhysics(this, std::move(impl)) {}

      PhysicalEntity(pEntityPhysicsImpl_t impl, long name, long type)
         : Entity(name, type), EntityPhysics(this, std::move(impl)) {}

      PhysicalEntity(const PhysicalEntity& copy)
         : Entity(copy), EntityPhysics(copy, this) {}

      PhysicalEntity(const PhysicalEntity& copy, long name)
         : Entity(copy, name), EntityPhysics(copy, this) {}

      virtual void addToWorld() {
         EntityPhysics::addToWorld();
      }

      virtual void removeFromWorld() {
         EntityPhysics::removeFromWorld();
      }

      virtual void update() {
         Entity::update();
      }
};

typedef boost::shared_ptr<PhysicalEntity> pPhysicalEntity_t;


}


#endif
