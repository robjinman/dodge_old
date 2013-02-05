#include <cstring>
#include <xml/XmlAttribute.hpp>
#include <definitions.hpp>


namespace Dodge {


//===========================================
// XmlAttribute::getInt
//===========================================
int XmlAttribute::getInt() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   int i;
   if (SSCANF(m_attr->value(), "%d", &i) != 1)
      throw XmlException("Expected integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlAttribute::getLong
//===========================================
long XmlAttribute::getLong() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   long i;
   if (SSCANF(m_attr->value(), "%ld", &i) != 1)
      throw XmlException("Expected long integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlAttribute::getFloat
//===========================================
float XmlAttribute::getFloat() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   float f;
   if (SSCANF(m_attr->value(), "%f", &f) != 1)
      throw XmlException("Expected float", __FILE__, __LINE__);

   return f;
}

//===========================================
// XmlAttribute::getDouble
//===========================================
double XmlAttribute::getDouble() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   double d;
   if (SSCANF(m_attr->value(), "%lf", &d) != 1)
      throw XmlException("Expected double", __FILE__, __LINE__);

   return d;
}

//===========================================
// XmlAttribute::getBool
//===========================================
bool XmlAttribute::getBool() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   if (strcmp(m_attr->value(), "true") == 0)
      return true;
   else if (strcmp(m_attr->value(), "false") == 0)
      return false;

   throw XmlException("Expected 'true' or 'false'", __FILE__, __LINE__);
}


}
