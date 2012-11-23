#include "GravityRegion.hpp"


using namespace Dodge;


//===========================================
// GravityRegion::GravityRegion
//===========================================
GravityRegion::GravityRegion(const XmlNode data)
   : Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Shape(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, GravityRegion);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class GravityRegion; ");
   }
}

//===========================================
// GravityRegion::GravityRegion
//===========================================
GravityRegion* GravityRegion::clone() const {
   return new GravityRegion(*this);
}

//===========================================
// GravityRegion::assignData
//===========================================
void GravityRegion::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, GravityRegion)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Shape") {
         Shape::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class GravityRegion; ");
      throw;
   }
}
