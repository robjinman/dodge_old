/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_ATTRIBUTE_HPP__
#define __XML_ATTRIBUTE_HPP__


#include <string>
#include <memory>
#include "../../rapidxml/rapidxml.hpp"
#include "XmlException.hpp"


namespace Dodge {


class XmlAttribute {
   public:
      XmlAttribute() : m_attr(NULL) {}

      XmlAttribute(std::weak_ptr<rapidxml::xml_document<> > doc, rapidxml::xml_attribute<>* attr, const std::string& file)
         : m_doc(doc), m_attr(attr), m_file(file) {}

      inline std::string name() const;
      inline std::string file() const;

      inline std::string getString() const;
      int getInt() const;
      long getLong() const;
      float getFloat() const;
      double getDouble() const;
      bool getBool() const;

      void setValue(const std::string& val);

      inline XmlAttribute nextAttribute() const;
      inline bool isNull() const;

   private:
      std::weak_ptr<rapidxml::xml_document<> > m_doc;
      rapidxml::xml_attribute<>* m_attr;
      std::string m_file;
};

//===========================================
// XmlAttribute::name
//===========================================
inline std::string XmlAttribute::name() const {
   if (isNull()) throw XmlException("Attribute is NULL", __FILE__, __LINE__);
   return std::string(m_attr->name(), m_attr->name_size());
}

//===========================================
// XmlAttribute::getString
//===========================================
inline std::string XmlAttribute::getString() const {
   if (isNull())
      throw XmlException("Attribute is NULL", __FILE__, __LINE__);

   return std::string(m_attr->value(), m_attr->value_size());
}

//===========================================
// XmlAttribute::nextAttribute
//===========================================
inline XmlAttribute XmlAttribute::nextAttribute() const {
   if (isNull()) throw XmlException("Attribute is NULL", __FILE__, __LINE__);
   return XmlAttribute(m_doc, m_attr->next_attribute(), m_file);
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
