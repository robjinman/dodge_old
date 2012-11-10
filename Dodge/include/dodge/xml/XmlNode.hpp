/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_NODE_HPP__
#define __XML_NODE_HPP__


#include "rapidxml/rapidxml.hpp"
#include "../definitions.hpp"
#include "XmlException.hpp"
#include "XmlAttribute.hpp"


namespace Dodge {


class XmlNode {
   public:
      explicit XmlNode(const rapidxml::xml_node<>* node) : m_node(node) {}

      inline std::string name() const;
      inline std::string value() const;
      inline const XmlNode firstChild() const;
      inline const XmlNode nthChild(uint_t n) const;
      inline const XmlNode nextSibling() const;
      inline const XmlAttribute firstAttribute() const;

      inline bool isNull() const;

   private:
      const rapidxml::xml_node<>* m_node;
};

inline std::string XmlNode::name() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return std::string(m_node->name());
}

inline std::string XmlNode::value() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return std::string(m_node->value());
}

inline const XmlNode XmlNode::firstChild() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlNode(m_node->first_node());
}

inline const XmlNode XmlNode::nthChild(uint_t n) const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);

   XmlNode c = firstChild();
   for (uint_t i = 0; i < n; ++i) c = c.nextSibling();

   return c;
}

inline const XmlNode XmlNode::nextSibling() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlNode(m_node->next_sibling());
}

inline const XmlAttribute XmlNode::firstAttribute() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlAttribute(m_node->first_attribute());
}

inline bool XmlNode::isNull() const {
   return m_node == NULL;
}


}


#endif
