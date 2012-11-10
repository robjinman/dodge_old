/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_ATTRIBUTE_HPP__
#define __XML_ATTRIBUTE_HPP__


#include <string>
#include "rapidxml/rapidxml.hpp"
#include "XmlException.hpp"


namespace Dodge {


class XmlAttribute {
   public:
      explicit XmlAttribute(const rapidxml::xml_attribute<>* attr) : m_attr(attr) {}

      inline std::string name() const;
      inline std::string value() const;
      inline const XmlAttribute nextAttribute() const;

      inline bool isNull() const;

   private:
      const rapidxml::xml_attribute<>* m_attr;
};

inline std::string XmlAttribute::name() const {
   if (isNull()) throw XmlException("Attribute is NULL", __FILE__, __LINE__);
   return std::string(m_attr->name());
}

inline std::string XmlAttribute::value() const {
   if (isNull()) throw XmlException("Attribute is NULL", __FILE__, __LINE__);
   return std::string(m_attr->value());
}

inline const XmlAttribute XmlAttribute::nextAttribute() const {
   if (isNull()) throw XmlException("Attribute is NULL", __FILE__, __LINE__);
   return XmlAttribute(m_attr->next_attribute());
}

inline bool XmlAttribute::isNull() const {
   return m_attr == NULL;
}


}


#endif
