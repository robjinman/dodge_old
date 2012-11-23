#ifndef __SOIL_HPP__
#define __SOIL_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Box2dPhysics.hpp>
#include <dodge/PhysicalSprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class Soil : public Item, public Dodge::PhysicalSprite<Dodge::Box2dPhysics> {
   public:
      Soil(const Dodge::XmlNode data);

      Soil(const Soil& copy)
         : Entity(copy),
           Item(copy),
           PhysicalSprite<Box2dPhysics>(copy) {}

      Soil(const Soil& copy, long name)
         : Entity(copy, name),
           Item(copy, name),
           PhysicalSprite<Box2dPhysics>(copy, name) {}

      virtual void draw() const;
      virtual void update();
      virtual Soil* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void assignData(const Dodge::XmlNode data);

      virtual ~Soil();
};

typedef boost::shared_ptr<Soil> pSoil_t;


#endif
