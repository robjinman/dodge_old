#ifndef __SOIL_HPP__
#define __SOIL_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Box2dPhysics.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class Soil : public Item, public Dodge::Box2dPhysics {
   public:
      Soil(const Dodge::XmlNode data);

      Soil(const Soil& copy)
         : Item(copy),
           Box2dPhysics(this, EntityPhysics::options_t(false, true, 1.0, 0.3)) {}

      Soil(const Soil& copy, long name)
         : Item(copy, name),
           Box2dPhysics(this, EntityPhysics::options_t(false, true, 1.0, 0.3)) {}

      virtual void update();
      virtual Soil* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void assignData(const Dodge::XmlNode data);

      virtual ~Soil() {}
};

typedef boost::shared_ptr<Soil> pSoil_t;


#endif
