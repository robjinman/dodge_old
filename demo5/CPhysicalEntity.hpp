#ifndef __C_PHYSICAL_ENTITY_HPP__
#define __C_PHYSICAL_ENTITY_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Box2dPhysics.hpp>
#include <dodge/PhysicalEntity.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class CPhysicalEntity : public Item, public Dodge::PhysicalEntity<Dodge::Box2dPhysics> {
   public:
      CPhysicalEntity(const Dodge::XmlNode data);
      CPhysicalEntity(const CPhysicalEntity& copy);
      CPhysicalEntity(const CPhysicalEntity& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual CPhysicalEntity* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);
//      virtual void onEvent(const Dodge::EEvent* event);

      virtual ~CPhysicalEntity() {}
};

typedef boost::shared_ptr<CPhysicalEntity> pCPhysicalEntity_t;


#endif
