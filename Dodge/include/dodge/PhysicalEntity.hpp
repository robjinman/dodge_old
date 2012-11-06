/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PHYSICAL_ENTITY_HPP__
#define __PHYSICAL_ENTITY_HPP__


#include <boost/shared_ptr.hpp>
#include "Entity.hpp"
#include "EntityPhysics.hpp"
#ifdef DEBUG
#include <ostream>
#include "rapidxml/rapidxml.hpp"
#endif
#include "xml.hpp"


namespace Dodge {


class PhysicalEntity : public Entity, public EntityPhysics {
   public:
      PhysicalEntity(const rapidxml::xml_node<>* data)
         : Entity(nthChild(data, 0)), EntityPhysics(nthChild(data, 1)) {}

      PhysicalEntity(pEntityPhysicsImpl_t impl, long type)
         : Entity(type), EntityPhysics(this, std::move(impl)) {}

      PhysicalEntity(pEntityPhysicsImpl_t impl, long name, long type)
         : Entity(name, type), EntityPhysics(this, std::move(impl)) {}

      PhysicalEntity(const PhysicalEntity& copy)
         : Entity(copy), EntityPhysics(copy, this) {}

      PhysicalEntity(const PhysicalEntity& copy, long name)
         : Entity(copy, name), EntityPhysics(copy, this) {}

      virtual PhysicalEntity* clone() const;
      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void addToWorld();
      virtual void removeFromWorld();
      virtual void update();
};

typedef boost::shared_ptr<PhysicalEntity> pPhysicalEntity_t;


}


#endif
