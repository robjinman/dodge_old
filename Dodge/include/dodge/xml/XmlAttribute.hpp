/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_ATTRIBUTE_HPP__
#define __XML_ATTRIBUTE_HPP__


#include <string>
#include "../../rapidxml/rapidxml.hpp"
#include "XmlException.hpp"


namespace Dodge {


class XmlAttribute {
   public:
      XmlAttribute() : m_attr(NULL) {}

      XmlAttribute(const rapidxml::xml_attribute<>* attr, const std::string& file)
         : m_attr(attr), m_file(file) {}

      inline std::string name() const;
      inline std::string file() const;

      inline std::string getString() const;
      int getInt() const;
      long getLong() const;
      float getFloat() const;
      double getDouble() const;
      bool getBool() const;

      inline const XmlAttribute nextAttribute() const;
      inline bool isNull() const;

   private:
      const rapidxml::xml_attribute<>* m_attr;
      std::string m_file;
};

//===========================================
// XmlAttribute::name
//===========================================
inline std::string XmlAttribute::name() const {
   if (isNull()) throw XmlException("Attribute is NULL", __FILE__, __LINE__);
   return std::string(m_attr->name());
}

//===========================================
// XmlAttribute::getString
//===========================================
inline std::string XmlAttribute::getString() const {
   if (isNull())
      throw XmlException("Attribute is NULL", __FILE__, __LINE__);

   return std::string(m_attr->value());
}

//===========================================
// XmlAttribute::nextAttribute
//===========================================
inline const XmlAttribute XmlAttribute::nextAttribute() const {
   if (isNull()) throw XmlException("Attribute is NULL", __FILE__, __LINE__);
   return XmlAttribute(m_attr->next_attribute(), m_file);
}

//===========================================
// XmlAttribute::isNull
//===========================================
inline bool XmlAttribute::isNull() const {
   return m_attr == NULL;
}

//===========================================
// XmlAttribute::file
//===========================================
inline std::string XmlAttribute::file() const {
   return m_file;
}


}


#endif
