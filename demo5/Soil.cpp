#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include "Soil.hpp"


using namespace Dodge;


//===========================================
// Soil::Soil
//===========================================
Soil::Soil(const XmlNode data)
   : Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     PhysicalSprite<Box2dPhysics>(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, Soil);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Soil; ");
   }
}

//===========================================
// Soil::clone
//===========================================
Soil* Soil::clone() const {
   return new Soil(*this);
}

//===========================================
// Soil::addToWorld
//===========================================
void Soil::addToWorld() {
   PhysicalSprite<Box2dPhysics>::addToWorld();
}

//===========================================
// Soil::removeFromWorld
//===========================================
void Soil::removeFromWorld() {
   PhysicalSprite<Box2dPhysics>::removeFromWorld();
}

//===========================================
// Soil::onEvent
//===========================================
void Soil::onEvent(const EEvent* event) {
   static long hitFromLeftStr = internString("hitFromLeft");
   static long hitFromTopStr = internString("hitFromTop");
   static long hitFromRightStr = internString("hitFromRight");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long dissolveStr = internString("dissolve");
   static long animFinishedStr = internString("animFinished");

   if (event->getType() == hitFromLeftStr
      || event->getType() == hitFromRightStr
      || event->getType() == hitFromTopStr
      || event->getType() == hitFromBottomStr) {

      playAnimation(dissolveStr);
   }
   else if (event->getType() == animFinishedStr) {
      const EAnimFinished* e = static_cast<const EAnimFinished*>(event);

      if (e->animation->getName() == dissolveStr)
         setPendingDeletion();
   }
}

//===========================================
// Soil::update
//===========================================
void Soil::update() {
   PhysicalSprite<Box2dPhysics>::update();
}

//===========================================
// Soil::draw
//===========================================
void Soil::draw() const {
   PhysicalSprite<Box2dPhysics>::draw();
}

//===========================================
// Soil::assignData
//===========================================
void Soil::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Soil)

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
      e.prepend("Error parsing XML for instance of class Soil; ");
      throw;
   }
}

//===========================================
// Soil::~Soil
//===========================================
Soil::~Soil() {}
