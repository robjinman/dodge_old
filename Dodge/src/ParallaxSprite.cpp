#include "ParallaxSprite.hpp"


using namespace std;


namespace Dodge {


//===========================================
// ParallaxSprite::ParallaxSprite
//===========================================
ParallaxSprite::ParallaxSprite(const XmlNode data)
   : Entity(data.firstChild().firstChild()),
     Sprite(data.nthChild(0)),
     EntityParallax(this, data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, ParallaxSprite);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class ParallaxSprite; ");
      throw;
   }
}

//===========================================
// ParallaxSprite::assignData
//===========================================
void ParallaxSprite::assignData(const XmlNode data) {
   try {
      if (data.isNull() || data.name() != "ParallaxSprite") return;

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "EntityParallax") {
         EntityParallax::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class ParallaxSprite; ");
      throw;
   }
}

//===========================================
// ParallaxSprite::clone
//===========================================
ParallaxSprite* ParallaxSprite::clone() const {
   return new ParallaxSprite(*this);
}

//===========================================
// ParallaxSprite::update
//===========================================
void ParallaxSprite::update() {
   Sprite::update();
   EntityParallax::update();
}

#ifdef DEBUG
//===========================================
// ParallaxSprite::dbg_print
//===========================================
void ParallaxSprite::dbg_print(ostream& cout, int tab) const {
   // TODO
}
#endif


}
