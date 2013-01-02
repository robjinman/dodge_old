#include "CPhysicalSprite.hpp"


using namespace Dodge;


//===========================================
// CPhysicalSprite::CPhysicalSprite
//===========================================
CPhysicalSprite::CPhysicalSprite(const XmlNode data)
   : Asset(internString("CPhysicalSprite")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     PhysicalSprite<Box2dPhysics>(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, CPhysicalSprite);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CPhysicalSprite; ");
   }
}

//===========================================
// CPhysicalSprite::CPhysicalSprite
//===========================================
CPhysicalSprite::CPhysicalSprite(const CPhysicalSprite& copy)
   : Asset(internString("CPhysicalSprite")),
     Entity(copy),
     Item(copy),
     PhysicalSprite<Box2dPhysics>(copy) {}

//===========================================
// CPhysicalSprite::CPhysicalSprite
//===========================================
CPhysicalSprite::CPhysicalSprite(const CPhysicalSprite& copy, long name)
   : Asset(internString("CPhysicalSprite")),
     Entity(copy, name),
     Item(copy, name),
     PhysicalSprite<Box2dPhysics>(copy, name) {}

//===========================================
// CPhysicalSprite::getSize
//===========================================
size_t CPhysicalSprite::getSize() const {
   return sizeof(CPhysicalSprite)
      - sizeof(Item)
      - sizeof(PhysicalSprite<Box2dPhysics>)
      + Item::getSize()
      + PhysicalSprite<Box2dPhysics>::getSize()
      - sizeof(Asset);
}

//===========================================
// CPhysicalSprite::clone
//===========================================
CPhysicalSprite* CPhysicalSprite::clone() const {
   return new CPhysicalSprite(*this);
}

//===========================================
// CPhysicalSprite::addToWorld
//===========================================
void CPhysicalSprite::addToWorld() {
   PhysicalSprite<Box2dPhysics>::addToWorld();
}

//===========================================
// CPhysicalSprite::removeFromWorld
//===========================================
void CPhysicalSprite::removeFromWorld() {
   PhysicalSprite<Box2dPhysics>::removeFromWorld();
}

//===========================================
// CPhysicalSprite::update
//===========================================
void CPhysicalSprite::update() {
   PhysicalSprite<Box2dPhysics>::update();
}

//===========================================
// CPhysicalSprite::draw
//===========================================
void CPhysicalSprite::draw() const {
   PhysicalSprite<Box2dPhysics>::draw();
}

//===========================================
// CPhysicalSprite::assignData
//===========================================
void CPhysicalSprite::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, CPhysicalSprite)

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
      e.prepend("Error parsing XML for instance of class CPhysicalSprite; ");
      throw;
   }
}
