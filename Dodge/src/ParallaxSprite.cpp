#include "ParallaxSprite.hpp"


using namespace std;


namespace Dodge {


//===========================================
// ParallaxSprite::ParallaxSprite
//===========================================
ParallaxSprite::ParallaxSprite(const XmlNode data)
   : Asset(internString("ParalaxSprite")),
     Entity(data.firstChild().firstChild()),
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
// ParallaxSprite::ParallaxSprite
//===========================================
ParallaxSprite::ParallaxSprite(const ParallaxSprite& copy)
   : Asset(internString("ParalaxSprite")),
     Entity(copy),
     Sprite(copy),
     EntityParallax(this, copy) {}

//===========================================
// ParallaxSprite::ParallaxSprite
//===========================================
ParallaxSprite::ParallaxSprite(const ParallaxSprite& copy, long name)
   : Asset(internString("ParalaxSprite")),
     Entity(copy, name),
     Sprite(copy, name),
     EntityParallax(this, copy) {}

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
// ParallaxSprite::getSize
//===========================================
size_t ParallaxSprite::getSize() const {
   return sizeof(ParallaxSprite)
      - sizeof(Sprite)
      - sizeof(EntityParallax)
      + Sprite::getSize(),
      + EntityParallax::getSize();
}

//===========================================
// ParallaxSprite::clone
//===========================================
Asset* ParallaxSprite::clone() const {
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
