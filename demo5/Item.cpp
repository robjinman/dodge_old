#include <dodge/EventManager.hpp>
#include "EPendingDeletion.hpp"
#include "Item.hpp"


using namespace Dodge;


//===========================================
// Item::Item
//===========================================
Item::Item(const XmlNode data)
   : Entity(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, Item);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, solid);
      m_solid = attr.getBool();
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
// Item::setPendingDeletion
//===========================================
void Item::setPendingDeletion() {
   EPendingDeletion* event = new EPendingDeletion(boost::dynamic_pointer_cast<Item>(getSharedPtr()));

   EventManager eventManager;
   eventManager.queueEvent(event);
}

//===========================================
// Item::assignData
//===========================================
void Item::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Item);

      XmlAttribute attr = data.firstAttribute();
      if (!attr.isNull() && attr.name() == "solid")
         m_solid = attr.getBool();

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Entity")
         Entity::assignData(node);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Item; ");
      throw;
   }
}

//===========================================
// Item::~Item
//===========================================
Item::~Item() {}
