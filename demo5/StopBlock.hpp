#ifndef __STOP_BLOCK_HPP__
#define __STOP_BLOCK_HPP__


#include <boost/shared_ptr.hpp>
#include <Dodge/physics/EntityPhysics.hpp>
#include "rapidxml.hpp"
#include "Item.hpp"


class StopBlock : public Item, public Dodge::EntityPhysics {
   public:
      StopBlock()
         : Item(),
           EntityPhysics(this, Dodge::physOpts_t(false, true, 1.0, 0.3)) {}

      StopBlock(const StopBlock& copy)
         : Item(copy),
           EntityPhysics(this, Dodge::physOpts_t(false, true, 1.0, 0.3)) {}

      virtual void addToWorld() { EntityPhysics::addToWorld(); }
      virtual void removeFromWorld() { EntityPhysics::removeFromWorld(); }

      virtual void assignData(const rapidxml::xml_node<>* data);

      virtual ~StopBlock() {}
};

typedef boost::shared_ptr<StopBlock> pStopBlock_t;


#endif
