#include "EPendingDeletion.hpp"
#include "Item.hpp"


using namespace Dodge;
using namespace rapidxml;


EventManager Item::eventManager = EventManager();


//===========================================
// Item::setPendingDeletion
//===========================================
void Item::setPendingDeletion() {
   EPendingDeletion* event = new EPendingDeletion(boost::dynamic_pointer_cast<Item>(getSharedPtr()));
   eventManager.queueEvent(event);
}

//===========================================
// Item::assignData
//===========================================
void Item::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "Item")
      throw Exception("Error parsing XML for instance of class Item; Expected 'Item' tag", __FILE__, __LINE__);

   Sprite::assignData(data.firstChild());
}
