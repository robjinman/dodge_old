#ifndef __STOP_BLOCK_HPP__
#define __STOP_BLOCK_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Box2dPhysics.hpp>
#include <dodge/xml/xml.hpp>
#include "Item.hpp"


class StopBlock : public Item, public Dodge::Box2dPhysics {
   public:
      StopBlock(const Dodge::XmlNode data);

      StopBlock(const StopBlock& copy, long name)
         : Item(copy, name),
           Box2dPhysics(this, EntityPhysics::options_t(false, true, 1.0, 0.3)) {}

      StopBlock(const StopBlock& copy)
         : Item(copy),
           Box2dPhysics(this, EntityPhysics::options_t(false, true, 1.0, 0.3)) {}

      virtual void addToWorld();
      virtual void removeFromWorld();
      virtual StopBlock* clone() const;
      virtual void assignData(const Dodge::XmlNode data);

      virtual ~StopBlock() {}
};

typedef boost::shared_ptr<StopBlock> pStopBlock_t;


#endif
