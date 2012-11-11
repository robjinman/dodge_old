/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PHYSICAL_ENTITY_HPP__
#define __PHYSICAL_ENTITY_HPP__


#include "Entity.hpp"
#include "EntityPhysics.hpp"
#ifdef DEBUG
#include <ostream>
#endif
#include "xml/xml.hpp"


namespace Dodge {


template <class T_PHYSICS>
class PhysicalEntity : public Entity, public T_PHYSICS {

   static_assert(
      std::is_base_of<EntityPhysics, T_PHYSICS>::value,
      "T_PHYSICS is not an implementation of EntityPhysics"
   );

   public:
      PhysicalEntity(const XmlNode data)
         : Entity(data.nthChild(0)), T_PHYSICS(data.nthChild(1), this) {

         if (data.isNull() || data.name() != "PhysicalEntity")
            throw XmlException("Error parsing XML for instance of class PhysicalEntity; Expected 'PhysicalEntity' tag", __FILE__, __LINE__);
      }

      PhysicalEntity(long type)
         : Entity(type), T_PHYSICS(this) {}

      PhysicalEntity(long name, long type)
         : Entity(name, type), T_PHYSICS(this) {}

      PhysicalEntity(long type, const EntityPhysics::options_t& options)
         : Entity(type), T_PHYSICS(this, options) {}

      PhysicalEntity(long name, long type, const EntityPhysics::options_t& options)
         : Entity(name, type), T_PHYSICS(this, options) {}

      PhysicalEntity(const PhysicalEntity& copy)
         : Entity(copy), T_PHYSICS(copy, this) {}

      PhysicalEntity(const PhysicalEntity& copy, long name)
         : Entity(copy, name), T_PHYSICS(copy, this) {}

      //===========================================
      // PhysicalEntity::clone
      //===========================================
      virtual PhysicalEntity* clone() const {
         return new PhysicalEntity(*this);
      }

      //===========================================
      // PhysicalEntity::assignData
      //===========================================
      virtual void assignData(const XmlNode data) {
         if (data.isNull() || data.name() != "PhysicalEntity") return;

         XmlNode node = data.firstChild();

         if (!node.isNull() && node.name() == "Entity") {
            Entity::assignData(node);
            node = node.nextSibling();
         }

         if (!node.isNull() && node.name() == "EntityPhysics") {
            T_PHYSICS::assignData(data.nthChild(1));
         }
      }

      #ifdef DEBUG
      //===========================================
      // PhysicalEntity::dbg_print
      //===========================================
      virtual void dbg_print(std::ostream& out, int tab) const {
         for (int i = 0; i < tab; ++i) out << "\t";
         out << "PhysicalEntity:\n";
         Entity::dbg_print(out, tab + 1);
         T_PHYSICS::dbg_print(out, tab + 1);
      }
      #endif

      //===========================================
      // PhysicalEntity::addToWorld
      //===========================================
      virtual void addToWorld() {
         T_PHYSICS::addToWorld();
      }

      //===========================================
      // PhysicalEntity::removeFromWorld
      //===========================================
      virtual void removeFromWorld() {
         T_PHYSICS::removeFromWorld();
      }

      //===========================================
      // PhysicalEntity::update
      //===========================================
      virtual void update() {
         Entity::update();
      }
};


}


#endif
