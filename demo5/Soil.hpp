#ifndef __SOIL_HPP__
#define __SOIL_HPP__


#include <boost/shared_ptr.hpp>
#include <Dodge/core/EEvent.hpp>
#include <Dodge/physics/EntityPhysics.hpp>
#include "rapidxml.hpp"
#include "Item.hpp"


class Soil : public Item, public Dodge::EntityPhysics {
   public:
      Soil()
         : Item(),
           EntityPhysics(this, Dodge::physOpts_t(false, true, 1.0, 0.3)) {}

      Soil(const Soil& copy)
         : Item(copy),
           EntityPhysics(this, Dodge::physOpts_t(false, true, 1.0, 0.3)) {}

      virtual void update();

      virtual void addToWorld();
      virtual void removeFromWorld() { EntityPhysics::removeFromWorld(); }

      virtual void onEvent(const Dodge::pEEvent_t event);
      virtual void assignData(const rapidxml::xml_node<>* data);

      virtual ~Soil() {}
};

typedef boost::shared_ptr<Soil> pSoil_t;


#endif
