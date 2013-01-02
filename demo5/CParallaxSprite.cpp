#include "CParallaxSprite.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// CParallaxSprite::CParallaxSprite
//===========================================
CParallaxSprite::CParallaxSprite(const XmlNode data)
   : Asset(internString("CParallaxSprite")),
     Entity(data.firstChild().firstChild()),
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
// CParallaxSprite::CParallaxSprite
//===========================================
CParallaxSprite::CParallaxSprite(const CParallaxSprite& copy)
   : Asset(internString("CParallaxSprite")),
     Entity(copy),
     Item(copy),
     Dodge::ParallaxSprite(copy) {}

//===========================================
// CParallaxSprite::CParallaxSprite
//===========================================
CParallaxSprite::CParallaxSprite(const CParallaxSprite& copy, long name)
   : Asset(internString("CParallaxSprite")),
     Entity(copy, name),
     Item(copy, name),
     Dodge::ParallaxSprite(copy) {}

//===========================================
// CParallaxSprite::getSize
//===========================================
size_t CParallaxSprite::getSize() const {
   return sizeof(CParallaxSprite)
      - sizeof(Item)
      - sizeof(ParallaxSprite)
      + Item::getSize()
      + ParallaxSprite::getSize()
      - sizeof(Asset);
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
