#include <dodge/EventManager.hpp>
#include "EPendingDeletion.hpp"
#include "Item.hpp"


using namespace Dodge;


//===========================================
// Item::Item
//===========================================
Item::Item(const XmlNode data)
   : Sprite(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, Item);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Item; ");
      throw;
   }
}

//===========================================
// Item::clone
//===========================================
Item* Item::clone() const {
   return new Item(*this);
}

//===========================================
// Item::draw
//===========================================
void Item::draw() const {
   Sprite::draw();
}

//===========================================
// Item::update
//===========================================
void Item::update() {
   Sprite::update();
}

//===========================================
// Item::setPendingDeletion
//===========================================
void Item::setPendingDeletion() {
   EPendingDeletion* event = new EPendingDeletion(boost::static_pointer_cast<Item>(getSharedPtr()));

   EventManager eventManager;
   eventManager.queueEvent(event);
}

//===========================================
// Item::assignData
//===========================================
void Item::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Item);

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Sprite")
         Sprite::assignData(node);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Item; ");
      throw;
   }
}
