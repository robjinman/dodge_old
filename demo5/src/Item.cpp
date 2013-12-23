#include <dodge/EventManager.hpp>
#include "EPendingDeletion.hpp"
#include "Item.hpp"


using namespace Dodge;


//===========================================
// Item::Item
//===========================================
Item::Item(const XmlNode data) {
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
// Item::Item
//===========================================
Item::Item(const Item& cpy) {
   m_solid = cpy.m_solid;
}

//===========================================
// Item::clone
//===========================================
Item* Item::clone() const {
   return new Item(*this);
}

//===========================================
// Item::getSize
//===========================================
size_t Item::getSize() const {
   return sizeof(Item);
}

//===========================================
// Item::setPendingDeletion
//===========================================
void Item::setPendingDeletion() {
   EPendingDeletion* event = new EPendingDeletion(m_entity->getSharedPtr());

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
