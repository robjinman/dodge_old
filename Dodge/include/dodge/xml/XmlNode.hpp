/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_NODE_HPP__
#define __XML_NODE_HPP__


#include "../../rapidxml/rapidxml.hpp"
#include "../definitions.hpp"
#include "XmlException.hpp"
#include "XmlAttribute.hpp"


namespace Dodge {


class XmlNode {
   public:
      XmlNode() : m_node(NULL) {}

      XmlNode(const rapidxml::xml_node<>* node, const std::string& file)
         : m_node(node), m_file(file) {}

      inline std::string name() const;
      inline std::string file() const;

      inline std::string getString() const;
      int getInt() const;
      long getLong() const;
      float getFloat() const;
      double getDouble() const;
      bool getBool() const;

      inline const XmlNode firstChild() const;
      const XmlNode nthChild(uint_t n) const;
      inline const XmlNode nextSibling() const;
      inline const XmlAttribute firstAttribute() const;

      inline bool isNull() const;

   private:
      const rapidxml::xml_node<>* m_node;
      std::string m_file;
};

//===========================================
// XmlNode::name
//===========================================
inline std::string XmlNode::name() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return std::string(m_node->name());
}

//===========================================
// XmlNode::getString
//===========================================
inline std::string XmlNode::getString() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   return std::string(m_node->value());
}

//===========================================
// XmlNode::firstChild
//===========================================
inline const XmlNode XmlNode::firstChild() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlNode(m_node->first_node(), m_file);
}

//===========================================
// XmlNode::nextSibling
//===========================================
inline const XmlNode XmlNode::nextSibling() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlNode(m_node->next_sibling(), m_file);
}

//===========================================
// XmlNode::firstAttribute
//===========================================
inline const XmlAttribute XmlNode::firstAttribute() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlAttribute(m_node->first_attribute(), m_file);
}

//===========================================
// XmlNode::isNull
//===========================================
inline bool XmlNode::isNull() const {
   return m_node == NULL;
}

//===========================================
// XmlNode::file
//===========================================
inline std::string XmlNode::file() const {
   return m_file;
}


}


#endif
