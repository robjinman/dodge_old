#include "StopBlock.hpp"


using namespace Dodge;


//===========================================
// StopBlock::StopBlock
//===========================================
StopBlock::StopBlock(const XmlNode data) : Item(data.firstChild()), Box2dPhysics(this, data.nthChild(1)) {
   try {
      XML_NODE_CHECK(data, StopBlock);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class StopBlock; ");
   }
}

//===========================================
// StopBlock::clone
//===========================================
StopBlock* StopBlock::clone() const {
   return new StopBlock(*this);
}

//===========================================
// StopBlock::addToWorld
//===========================================
void StopBlock::addToWorld() {
   Box2dPhysics::addToWorld();
}

//===========================================
// StopBlock::removeFromWorld
//===========================================
void StopBlock::removeFromWorld() {
   Box2dPhysics::removeFromWorld();
}

//===========================================
// StopBlock::assignData
//===========================================
void StopBlock::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, StopBlock)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "StopBlock")
         Item::assignData(node);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class StopBlock; ");
      throw;
   }
}
