#include <cstring>
#include <sstream>
#include <xml/XmlAttribute.hpp>
#include <definitions.hpp>


using namespace std;


namespace Dodge {


//===========================================
// XmlAttribute::getInt
//===========================================
int XmlAttribute::getInt() const {
   if (isNull())
      throw XmlException("Attribute is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_attr->value(), m_attr->value_size());

   int i;
   ss >> i;

   if (ss.fail())
      throw XmlException("Expected integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlAttribute::getLong
//===========================================
long XmlAttribute::getLong() const {
   if (isNull())
      throw XmlException("Attribute is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_attr->value(), m_attr->value_size());

   long i;
   ss >> i;

   if (ss.fail())
      throw XmlException("Expected long integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlAttribute::getFloat
//===========================================
float XmlAttribute::getFloat() const {
   if (isNull())
      throw XmlException("Attribute is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_attr->value(), m_attr->value_size());

   float f;
   ss >> f;

   if (ss.fail())
      throw XmlException("Expected float", __FILE__, __LINE__);

   return f;
}

//===========================================
// XmlAttribute::getDouble
//===========================================
double XmlAttribute::getDouble() const {
   if (isNull())
      throw XmlException("Attribute is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_attr->value(), m_attr->value_size());

   double d;
   ss >> d;

   if (ss.fail())
      throw XmlException("Expected double", __FILE__, __LINE__);

   return d;
}

//===========================================
// XmlAttribute::getBool
//===========================================
bool XmlAttribute::getBool() const {
   if (isNull())
      throw XmlException("Attribute is NULL", __FILE__, __LINE__);

   if (strncmp(m_attr->value(), "true", m_attr->value_size()) == 0)
      return true;
   else if (strncmp(m_attr->value(), "false", m_attr->value_size()) == 0)
      return false;

   throw XmlException("Expected 'true' or 'false'", __FILE__, __LINE__);
}

//===========================================
// XmlAttribute::setValue
//===========================================
void XmlAttribute::setValue(const string& val) {
   auto doc = m_doc.lock();
   assert(doc);

   char* str = doc->allocate_string(val.data(), val.length());
   m_attr->value(str, val.length());
}


}
