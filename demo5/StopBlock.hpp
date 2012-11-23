#ifndef __STOP_BLOCK_HPP__
#define __STOP_BLOCK_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Box2dPhysics.hpp>
#include <dodge/PhysicalEntity.hpp>
#include <dodge/xml/xml.hpp>
#include "Item.hpp"


class StopBlock : public Item, public Dodge::PhysicalEntity<Dodge::Box2dPhysics> {
   public:
      StopBlock(const Dodge::XmlNode data);

      StopBlock(const StopBlock& copy, long name)
         : Entity(copy, name),
           Item(copy, name),
           PhysicalEntity<Box2dPhysics>(copy, name) {}

      StopBlock(const StopBlock& copy)
         : Entity(copy),
           Item(copy),
           PhysicalEntity<Box2dPhysics>(copy) {}

      virtual void draw() const;
      virtual void update();
      virtual void addToWorld();
      virtual void removeFromWorld();
      virtual StopBlock* clone() const;
      virtual void assignData(const Dodge::XmlNode data);

      virtual ~StopBlock() {}
};

typedef boost::shared_ptr<StopBlock> pStopBlock_t;


#endif
