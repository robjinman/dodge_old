/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <renderer/ogles2.0/Colour.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// Colour::Colour
//===========================================
Colour::Colour(const XmlNode data) {
   if (data.isNull() || data.name() != "Colour")
      throw XmlException("Error parsing XML for instance of class Colour; Expected 'Colour' tag", __FILE__, __LINE__);

   XmlAttribute attr = data.firstAttribute();

   if (attr.isNull() || attr.name() != "r")
      throw XmlException("Error parsing XML for instance of class Colour; Expected 'r' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &r);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "g")
      throw XmlException("Error parsing XML for instance of class Colour; Expected 'g' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &g);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "b")
      throw XmlException("Error parsing XML for instance of class Colour; Expected 'b' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &b);
   attr = attr.nextAttribute();

   if (attr.isNull() || attr.name() != "a")
      throw XmlException("Error parsing XML for instance of class Colour; Expected 'a' attribute", __FILE__, __LINE__);

   sscanf(attr.value().data(), "%f", &a);
}


}
