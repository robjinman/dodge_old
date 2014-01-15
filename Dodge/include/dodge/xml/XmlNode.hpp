/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_NODE_HPP__
#define __XML_NODE_HPP__


#include <memory>
#include "../../rapidxml/rapidxml.hpp"
#include "../definitions.hpp"
#include "XmlException.hpp"
#include "XmlAttribute.hpp"


namespace Dodge {


class XmlDocument;

class XmlNode {
   friend class XmlDocument;

   public:
      XmlNode() : m_node(NULL) {}
      XmlNode(XmlDocument& doc, const std::string& name);

      XmlNode copy(XmlDocument& doc) const;

      inline std::string name() const;
      inline std::string file() const;

      inline std::string getString() const;
      int getInt() const;
      long getLong() const;
      float getFloat() const;
      double getDouble() const;
      bool getBool() const;

      void setValue(const std::string& val);

      inline XmlNode firstChild() const;
      XmlNode nthChild(uint_t n) const;
      inline XmlNode nextSibling() const;
      inline XmlAttribute firstAttribute() const;

      XmlNode addNode(const std::string& name);
      XmlNode addNode(const XmlNode& node);
      XmlAttribute addAttribute(const std::string& name, const std::string& value);

      inline bool isNull() const;

   private:
      XmlNode(std::weak_ptr<rapidxml::xml_document<> > doc, rapidxml::xml_node<>* node, const std::string& file)
         : m_doc(doc), m_node(node), m_file(file) {}

      static rapidxml::xml_node<>* copy_r(rapidxml::xml_node<>* node, rapidxml::xml_document<char>* doc);

      std::weak_ptr<rapidxml::xml_document<> > m_doc;
      rapidxml::xml_node<>* m_node;
      std::string m_file;
};

//===========================================
// XmlNode::name
//===========================================
inline std::string XmlNode::name() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return std::string(m_node->name(), m_node->name_size());
}

//===========================================
// XmlNode::getString
//===========================================
inline std::string XmlNode::getString() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   return std::string(m_node->value(), m_node->value_size());
}

//===========================================
// XmlNode::firstChild
//===========================================
inline XmlNode XmlNode::firstChild() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlNode(m_doc, m_node->first_node(), m_file);
}

//===========================================
// XmlNode::nextSibling
//===========================================
inline XmlNode XmlNode::nextSibling() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlNode(m_doc, m_node->next_sibling(), m_file);
}

//===========================================
// XmlNode::firstAttribute
//===========================================
inline XmlAttribute XmlNode::firstAttribute() const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);
   return XmlAttribute(m_doc, m_node->first_attribute(), m_file);
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
