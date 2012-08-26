#include <dodge/rapidxml/rapidxml.hpp>
#include "EPendingDeletion.hpp"
#include "Item.hpp"


using namespace Dodge;
using namespace rapidxml;


EventManager Item::eventManager = EventManager();


//===========================================
// Item::setPendingDeletion
//===========================================
void Item::setPendingDeletion() {
   pEPendingDeletion_t event(new EPendingDeletion(boost::static_pointer_cast<Item>(getSharedPtr())));
   eventManager.queueEvent(event);
}

//===========================================
// Item::assignData
//===========================================
void Item::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Item") != 0)
      throw Exception("Error parsing XML for instance of class Item", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();
   if (node) Sprite::assignData(node);
}
