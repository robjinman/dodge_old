#include <dodge/StringId.hpp>
#include <dodge/EAnimFinished.hpp>
#include "Item.hpp"
#include "Soil.hpp"


using namespace Dodge;


//===========================================
// Soil::Soil
//===========================================
Soil::Soil(const XmlNode data)
   : Asset(internString("Soil")),
     Entity(data.firstChild().firstChild()),
     Sprite(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, Soil);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Soil; ");
   }
}

//===========================================
// Soil::Soil
//===========================================
Soil::Soil(const Soil& copy)
   : Asset(internString("Soil")),
     Entity(copy),
     Sprite(copy) {}

//===========================================
// Soil::Soil
//===========================================
Soil::Soil(const Soil& copy, long name)
   : Asset(internString("Soil")),
     Entity(copy, name),
     Sprite(copy, name) {}

//===========================================
// Soil::getSize
//===========================================
size_t Soil::getSize() const {
   return sizeof(Soil)
      - sizeof(Sprite)
      + Sprite::getSize();
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
   Sprite::addToWorld();
}

//===========================================
// Soil::removeFromWorld
//===========================================
void Soil::removeFromWorld() {
   Sprite::removeFromWorld();
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

   Sprite::onEvent(event);

   if (event->getType() == hitFromLeftStr
      || event->getType() == hitFromRightStr
      || event->getType() == hitFromTopStr
      || event->getType() == hitFromBottomStr) {

      playAnimation(dissolveStr);
   }
   else if (event->getType() == animFinishedStr) {
      const EAnimFinished* e = static_cast<const EAnimFinished*>(event);

      if (e->animation->getName() == dissolveStr) {
         Item* item = dynamic_cast<Item*>(getAuxDataPtr());
         assert(item);

         item->setPendingDeletion();
      }
   }
}

//===========================================
// Soil::update
//===========================================
void Soil::update() {
   Sprite::update();
}

//===========================================
// Soil::draw
//===========================================
void Soil::draw() const {
   Sprite::draw();
}

//===========================================
// Soil::assignData
//===========================================
void Soil::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Soil)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
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
