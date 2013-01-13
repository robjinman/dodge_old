/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <dodge/EventManager.hpp>
#include "EPendingDeletion.hpp"
#include "Item.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Item::Item
//===========================================
Item::Item(const XmlNode data)
   : Asset(internString("Item")),
     Entity(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, Item);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Item; ");
      throw;
   }
}

//===========================================
// Item::Item
//===========================================
Item::Item(const Item& item)
   : Asset(internString("Item")), Entity(item) {}

//===========================================
// Item::Item
//===========================================
Item::Item(const Item& item, long name)
   : Asset(internString("Item")), Entity(item, name) {}

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
   return sizeof(Item)
      - sizeof(Entity)
      + Entity::getSize();
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

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Entity")
         Entity::assignData(node);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Item; ");
      throw;
   }
}

#ifdef DEBUG
//===========================================
// Item::dbg_print
//===========================================
void Item::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Item\n";
   Entity::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Item::~Item
//===========================================
Item::~Item() {}
