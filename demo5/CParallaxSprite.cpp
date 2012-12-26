#include "CParallaxSprite.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// CParallaxSprite::CParallaxSprite
//===========================================
CParallaxSprite::CParallaxSprite(const XmlNode data)
   : Entity(data.firstChild().firstChild()),
     Item(data.nthChild(0)),
     ParallaxSprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, CParallaxSprite);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CParallaxSprite; ");
      throw;
   }
}

//===========================================
// CParallaxSprite::assignData
//===========================================
void CParallaxSprite::assignData(const XmlNode data) {
   try {
      if (data.isNull() || data.name() != "CParallaxSprite") return;

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "ParallaxSprite") {
         ParallaxSprite::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CParallaxSprite; ");
      throw;
   }
}

//===========================================
// CParallaxSprite::clone
//===========================================
CParallaxSprite* CParallaxSprite::clone() const {
   return new CParallaxSprite(*this);
}

//===========================================
// CParallaxSprite::update
//===========================================
void CParallaxSprite::update() {
   ParallaxSprite::update();
}

#ifdef DEBUG
//===========================================
// CParallaxSprite::dbg_print
//===========================================
void CParallaxSprite::dbg_print(ostream& cout, int tab) const {
   // TODO
}
#endif
