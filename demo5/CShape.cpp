#include "CShape.hpp"


using namespace Dodge;


//===========================================
// CShape::CShape
//===========================================
CShape::CShape(const XmlNode data)
   : Entity(data.firstChild().firstChild()),
     Item(data.firstChild()),
     Shape(data.nthChild(1)) {

   try {
      XML_NODE_CHECK(data, CShape);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class CShape; ");
   }
}

//===========================================
// CShape::CShape
//===========================================
CShape* CShape::clone() const {
   return new CShape(*this);
}

//===========================================
// CShape::assignData
//===========================================
void CShape::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, CShape)

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
      e.prepend("Error parsing XML for instance of class CShape; ");
      throw;
   }
}
