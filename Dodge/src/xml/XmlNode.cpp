#include <cstring>
#include <xml/XmlNode.hpp>
#include <definitions.hpp>


namespace Dodge {


//===========================================
// XmlNode::getInt
//===========================================
int XmlNode::getInt() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   int i;
   if (SSCANF(m_node->value(), "%d", &i) != 1)
      throw XmlException("Expected integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlNode::getLong
//===========================================
long XmlNode::getLong() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   long i;
   if (SSCANF(m_node->value(), "%ld", &i) != 1)
      throw XmlException("Expected long integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlNode::getFloat
//===========================================
float XmlNode::getFloat() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   float f;
   if (SSCANF(m_node->value(), "%f", &f) != 1)
      throw XmlException("Expected float", __FILE__, __LINE__);

   return f;
}

//===========================================
// XmlNode::getDouble
//===========================================
double XmlNode::getDouble() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   double d;
   if (SSCANF(m_node->value(), "%lf", &d) != 1)
      throw XmlException("Expected double", __FILE__, __LINE__);

   return d;
}

//===========================================
// XmlNode::getBool
//===========================================
bool XmlNode::getBool() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   if (strcmp(m_node->value(), "true") == 0)
      return true;
   else if (strcmp(m_node->value(), "false") == 0)
      return false;

   throw XmlException("Expected 'true' or 'false'", __FILE__, __LINE__);
}

//===========================================
// XmlNode::nthChild
//===========================================
const XmlNode XmlNode::nthChild(uint_t n) const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);

   XmlNode c = firstChild();
   for (uint_t i = 0; i < n; ++i) c = c.nextSibling();

   return c;
}


}
