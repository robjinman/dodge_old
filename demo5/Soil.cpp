#include <Dodge/core/StringId.hpp>
#include <Dodge/core/EAnimFinished.hpp>
#include "Soil.hpp"
#include "rapidxml.hpp"


using namespace Dodge;
using namespace rapidxml;


//===========================================
// Soil::addToWorld
//===========================================
void Soil::addToWorld() {
   EntityPhysics::addToWorld();
}

//===========================================
// Soil::onEvent
//===========================================
void Soil::onEvent(const pEEvent_t event) {
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
      pEAnimFinished_t e = boost::static_pointer_cast<EAnimFinished>(event);

      if (e->getAnimation()->getName() == dissolveStr)
         setPendingDeletion();
   }
}

//===========================================
// Soil::update
//===========================================
void Soil::update() {
   Item::update();
}

//===========================================
// Soil::assignData
//===========================================
void Soil::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Soil") != 0)
      throw Exception("Error parsing XML for instance of class Soil", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();
   if (node) Item::assignData(node);
}
