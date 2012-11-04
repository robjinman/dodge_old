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


namespace Dodge {


class PhysicalEntity : public Entity, public EntityPhysics {
   public:
      PhysicalEntity(pEntityPhysicsImpl_t impl, long type);
      PhysicalEntity(pEntityPhysicsImpl_t impl, long name, long type);
      PhysicalEntity(const PhysicalEntity& copy);
      PhysicalEntity(const PhysicalEntity& copy, long name);

      // TODO
      virtual PhysicalEntity* clone() const { return NULL; }

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
