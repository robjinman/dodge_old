#include "Cave.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Cave::Cave
//===========================================
Cave::Cave(const XmlNode data)
   : Entity(data.firstChild().firstChild().firstChild()),
     CSprite(data.nthChild(0)),
     EntityParallax(this, data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, Cave);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Cave; ");
      throw;
   }
}

//===========================================
// Cave::assignData
//===========================================
void Cave::assignData(const XmlNode data) {
   try {
      if (data.isNull() || data.name() != "Cave") return;

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "CSprite") {
         CSprite::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "EntityParallax") {
         EntityParallax::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Cave; ");
      throw;
   }
}

//===========================================
// Cave::clone
//===========================================
Cave* Cave::clone() const {
   return new Cave(*this);
}

//===========================================
// Cave::update
//===========================================
void Cave::update() {
   CSprite::update();
   EntityParallax::update();
}

#ifdef DEBUG
//===========================================
// Cave::dbg_print
//===========================================
void Cave::dbg_print(ostream& cout, int tab) const {
   // TODO
}
#endif
