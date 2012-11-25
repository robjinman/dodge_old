#include "CSprite.hpp"


using namespace std;
using namespace Dodge;


CSprite::CSprite(const Dodge::XmlNode data)
   : Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Sprite(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, CSprite);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CSprite; ");
   }
}

void CSprite::draw() const {
   Sprite::draw();
}

void CSprite::update() {
   Item::update();
   Sprite::update();
}

CSprite* CSprite::clone() const {
   return new CSprite(*this);
}

void CSprite::addToWorld() {
   Item::addToWorld();
   Sprite::addToWorld();
}

void CSprite::removeFromWorld() {
   Item::removeFromWorld();
   Sprite::removeFromWorld();
}

void CSprite::assignData(const Dodge::XmlNode data) {
   try {
      XML_NODE_CHECK(data, CSprite)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CSprite; ");
      throw;
   }
}
