#include "CPhysicalEntity.hpp"


using namespace Dodge;


//===========================================
// CPhysicalEntity::CPhysicalEntity
//===========================================
CPhysicalEntity::CPhysicalEntity(const XmlNode data)
   : Entity(data.firstChild().firstChild()),
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
// CPhysicalEntity::clone
//===========================================
CPhysicalEntity* CPhysicalEntity::clone() const {
   return new CPhysicalEntity(*this);
}

//===========================================
// CPhysicalEntity::addToWorld
//===========================================
void CPhysicalEntity::addToWorld() {
   PhysicalEntity<Box2dPhysics>::addToWorld();
   Item::addToWorld();
}

//===========================================
// CPhysicalEntity::removeFromWorld
//===========================================
void CPhysicalEntity::removeFromWorld() {
   PhysicalEntity<Box2dPhysics>::removeFromWorld();
   Item::removeFromWorld();
}

//===========================================
// CPhysicalEntity::update
//===========================================
void CPhysicalEntity::update() {
   PhysicalEntity<Box2dPhysics>::update();
   Item::update();
}

//===========================================
// CPhysicalEntity::render
//===========================================
void CPhysicalEntity::render() const {
   PhysicalEntity<Box2dPhysics>::render();
}

//===========================================
// CPhysicalEntity::unrender
//===========================================
void CPhysicalEntity::unrender() const {
   PhysicalEntity<Box2dPhysics>::unrender();
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
