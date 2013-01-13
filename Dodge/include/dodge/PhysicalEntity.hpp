/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PHYSICAL_ENTITY_HPP__
#define __PHYSICAL_ENTITY_HPP__


#include <memory>
#include "Entity.hpp"
#include "EntityPhysics.hpp"
#ifdef DEBUG
#include <ostream>
#endif
#include "xml/xml.hpp"


namespace Dodge {


template <class T_PHYSICS>
class PhysicalEntity : virtual public Entity, public T_PHYSICS {

   static_assert(
      std::is_base_of<EntityPhysics, T_PHYSICS>::value,
      "T_PHYSICS is not an implementation of EntityPhysics"
   );

   public:
      //===========================================
      // PhysicalEntity::PhysicalEntity
      //===========================================
      explicit PhysicalEntity(const XmlNode data)
         : Asset(internString("PhysicalEntity")), Entity(data.nthChild(0)), T_PHYSICS(this, data.nthChild(1)) {

         XML_NODE_CHECK(data, PhysicalEntity);
      }

      //===========================================
      // PhysicalEntity::PhysicalEntity
      //===========================================
      explicit PhysicalEntity(long type, std::unique_ptr<Shape> shape)
         : Asset(internString("PhysicalEntity")), Entity(type), T_PHYSICS(this) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalEntity::PhysicalEntity
      //===========================================
      PhysicalEntity(long name, long type, std::unique_ptr<Shape> shape)
         : Asset(internString("PhysicalEntity")), Entity(name, type), T_PHYSICS(this) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalEntity::PhysicalEntity
      //===========================================
      PhysicalEntity(long type, std::unique_ptr<Shape> shape, const EntityPhysics::options_t& options)
         : Asset(internString("PhysicalEntity")), Entity(type), T_PHYSICS(this, options) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalEntity::PhysicalEntity
      //===========================================
      PhysicalEntity(long name, long type, std::unique_ptr<Shape> shape, const EntityPhysics::options_t& options)
         : Asset(internString("PhysicalEntity")), Entity(name, type), T_PHYSICS(this, options) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalEntity::PhysicalEntity
      //===========================================
      PhysicalEntity(const PhysicalEntity& copy)
         : Asset(internString("PhysicalEntity")), Entity(copy), T_PHYSICS(copy, this) {}

      //===========================================
      // PhysicalEntity::PhysicalEntity
      //===========================================
      PhysicalEntity(const PhysicalEntity& copy, long name)
         : Asset(internString("PhysicalEntity")), Entity(copy, name), T_PHYSICS(copy, this) {}

      //===========================================
      // PhysicalEntity::getSize
      //===========================================
      virtual size_t getSize() const {
         return Entity::getSize() + T_PHYSICS::getSize();
      }

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

      //===========================================
      // PhysicalEntity::onEvent
      //===========================================
      virtual void onEvent(const EEvent* event) {
         Entity::onEvent(event);
      }

      #ifdef DEBUG
      //===========================================
      // PhysicalEntity::dbg_print
      //===========================================
      virtual void dbg_print(std::ostream& out, int tab = 0) const {
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
         Entity::addToWorld();
         T_PHYSICS::addToWorld();
      }

      //===========================================
      // PhysicalEntity::removeFromWorld
      //===========================================
      virtual void removeFromWorld() {
         Entity::removeFromWorld();
         T_PHYSICS::removeFromWorld();
      }

      //===========================================
      // PhysicalEntity::update
      //===========================================
      virtual void update() {
         Entity::update();
      }

      //===========================================
      // PhysicalEntity::~PhysicalEntity
      //===========================================
      virtual ~PhysicalEntity() {}
};


}


#endif
