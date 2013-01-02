#include "CPhysicalEntity.hpp"


using namespace Dodge;


//===========================================
// CPhysicalEntity::CPhysicalEntity
//===========================================
CPhysicalEntity::CPhysicalEntity(const XmlNode data)
   : Asset(internString("CPhysicalEntity")),
     Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     PhysicalEntity<Box2dPhysics>(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, CPhysicalEntity);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CPhysicalEntity; ");
   }
}

//===========================================
// CPhysicalEntity::CPhysicalEntity
//===========================================
CPhysicalEntity::CPhysicalEntity(const CPhysicalEntity& copy)
   : Asset(internString("CPhysicalEntity")),
     Entity(copy),
     Item(copy),
     PhysicalEntity<Box2dPhysics>(copy) {}

//===========================================
// CPhysicalEntity::CPhysicalEntity
//===========================================
CPhysicalEntity::CPhysicalEntity(const CPhysicalEntity& copy, long name)
   : Asset(internString("CPhysicalEntity")),
     Entity(copy, name),
     Item(copy, name),
     PhysicalEntity<Box2dPhysics>(copy, name) {}

//===========================================
// CPhysicalEntity::clone
//===========================================
CPhysicalEntity* CPhysicalEntity::clone() const {
   return new CPhysicalEntity(*this);
}

//===========================================
// CPhysicalEntity::getSize
//===========================================
size_t CPhysicalEntity::getSize() const {
   return sizeof(CPhysicalEntity)
      - sizeof(Item)
      - sizeof(PhysicalEntity<Box2dPhysics>)
      + Item::getSize()
      + PhysicalEntity<Box2dPhysics>::getSize();
}

//===========================================
// CPhysicalEntity::addToWorld
//===========================================
void CPhysicalEntity::addToWorld() {
   PhysicalEntity<Box2dPhysics>::addToWorld();
}

//===========================================
// CPhysicalEntity::removeFromWorld
//===========================================
void CPhysicalEntity::removeFromWorld() {
   PhysicalEntity<Box2dPhysics>::removeFromWorld();
}

//===========================================
// CPhysicalEntity::update
//===========================================
void CPhysicalEntity::update() {
   PhysicalEntity<Box2dPhysics>::update();
}

//===========================================
// CPhysicalEntity::draw
//===========================================
void CPhysicalEntity::draw() const {
   PhysicalEntity<Box2dPhysics>::draw();
}

//===========================================
// CPhysicalEntity::assignData
//===========================================
void CPhysicalEntity::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, CPhysicalEntity)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "PhysicalEntity") {
         PhysicalEntity<Box2dPhysics>::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CPhysicalEntity; ");
      throw;
   }
}
