/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <renderer/ogl/Colour.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Colour::Colour
//===========================================
Colour::Colour(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Colour);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, r);
      r = attr.getFloat();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, g);
      g = attr.getFloat();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, b);
      b = attr.getFloat();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, a);
      a = attr.getFloat();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Colour; ");
      throw;
   }
}


}
