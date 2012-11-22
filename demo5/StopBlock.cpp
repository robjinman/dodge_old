#include "StopBlock.hpp"


using namespace Dodge;


//===========================================
// StopBlock::StopBlock
//===========================================
StopBlock::StopBlock(const XmlNode data)
   : Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     PhysicalSprite<Box2dPhysics>(data.nthChild(1)) {

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
   PhysicalSprite<Box2dPhysics>::addToWorld();
}

//===========================================
// StopBlock::removeFromWorld
//===========================================
void StopBlock::removeFromWorld() {
   PhysicalSprite<Box2dPhysics>::removeFromWorld();
}

//===========================================
// StopBlock::update
//===========================================
void StopBlock::update() {
   PhysicalSprite<Box2dPhysics>::update();
}

//===========================================
// StopBlock::draw
//===========================================
void StopBlock::draw() const {
   PhysicalSprite<Box2dPhysics>::draw();
}

//===========================================
// StopBlock::assignData
//===========================================
void StopBlock::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, StopBlock)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "PhysicalSprite") {
         PhysicalSprite<Box2dPhysics>::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class StopBlock; ");
      throw;
   }
}
