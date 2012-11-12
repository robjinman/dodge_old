/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <renderer/ogles2.0/Colour.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Colour::Colour
//===========================================
Colour::Colour(const XmlNode data) {
   string msg("Error parsing XML for instance of class Colour");

   XML_NODE_CHECK(msg, data, Colour);

   XmlAttribute attr = data.firstAttribute();
   XML_ATTR_CHECK(msg, attr, r);
   sscanf(attr.value().data(), "%f", &r);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, g);
   sscanf(attr.value().data(), "%f", &g);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, b);
   sscanf(attr.value().data(), "%f", &b);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, a);
   sscanf(attr.value().data(), "%f", &a);
}


}
